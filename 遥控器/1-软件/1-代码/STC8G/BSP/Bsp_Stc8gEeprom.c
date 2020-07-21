#include "Bsp_Stc8gEeprom.h"
#include "Bsp_Stc8gUart.h"
#include "App_GlobalValue.h"

void Bsp_Stc8gEeprom_Init(void)
{
	IapEeprom_Idle();//空闲	
}
/**
 *************************************************************************
 * @brief: IapEeprom_Idle
 * @param: void
 * @return: void
 * @function:
 * @author: 
 * @version: V1.0
 * @note:
 *************************************************************************
**/
void IapEeprom_Idle(void)
{
	IAP_CONTR = 0; //关闭 IAP 功能
	IAP_CMD = 0; //清除命令寄存器
	IAP_TRIG = 0; //清除触发寄存器
	IAP_ADDRH = 0x80; //将地址设置到非 IAP 区域
	IAP_ADDRL = 0;
}
/**
 *************************************************************************
 * @brief: IapEeprom_Read
 * @param: void
 * @return: void
 * @function:
 * @author: 
 * @version: V1.0
 * @note:
 *************************************************************************
**/
INT8U IapEeprom_Read(INT16U _uin_Addr)
{
	INT8U _uch_Data;
	IAP_CONTR = 0x80; //使能 IAP
	IAP_TPS = 12; //设置擦除等待参数 12MHz
	IAP_CMD = 1; //设置 IAP 读命令
	IAP_ADDRL = _uin_Addr; //设置 IAP 低地址
	IAP_ADDRH = _uin_Addr >> 8; //设置 IAP 高地址
	IAP_TRIG = 0x5a; //写触发命令(0x5a)
	IAP_TRIG = 0xa5; //写触发命令(0xa5)
	_nop_();
	_uch_Data = IAP_DATA; //读 IAP 数据
	IapEeprom_Idle(); //关闭 IAP 功能
	return _uch_Data;
}
/**
 *************************************************************************
 * @brief: IapEeprom_Program
 * @param:  _uin_Addr -- 地址 _uch_Data -- 写入的数据
 * @return: void
 * @function:
 * @author: 
 * @version: V1.0
 * @note:
 *************************************************************************
**/
void IapEeprom_Program(INT16U _uin_Addr, INT8U _uch_Data)
{
	IAP_CONTR = 0x80; //使能 IAP
	IAP_TPS = 12; //设置擦除等待参数 12MHz
	IAP_CMD = 2; //设置 IAP 写命令
	IAP_ADDRL = _uin_Addr; //设置 IAP 低地址
	IAP_ADDRH = _uin_Addr >> 8; //设置 IAP 高地址
	IAP_DATA = _uch_Data; //写 IAP 数据
	IAP_TRIG = 0x5a; //写触发命令(0x5a)
	IAP_TRIG = 0xa5; //写触发命令(0xa5)
	_nop_();
	IapEeprom_Idle(); //关闭 IAP 功能
}
/**
 *************************************************************************
 * @brief: IapEeprom_Erase
 * @param: _uin_Addr -- 地址
 * @return: void
 * @function:
 * @author: 
 * @version: V1.0
 * @note:
 *************************************************************************
**/
void IapEeprom_Erase(INT16U _uin_Addr)
{
	IAP_CONTR = 0x80; //使能 IAP
	IAP_TPS = 12; //设置擦除等待参数 12MHz
	IAP_CMD = 3; //设置 IAP 擦除命令
	IAP_ADDRL = _uin_Addr; //设置 IAP 低地址
	IAP_ADDRH = _uin_Addr >> 8; //设置 IAP 高地址
	IAP_TRIG = 0x5a; //写触发命令(0x5a)
	IAP_TRIG = 0xa5; //写触发命令(0xa5)
	_nop_(); //
	IapEeprom_Idle(); //关闭 IAP 功能
}
/**
 *************************************************************************
 * @brief: Eeprom_Test
 * @param: void
 * @return: void
 * @function:
 * @author: 
 * @version: V1.0
 * @note:
 *************************************************************************
**/
void Eeprom_Test(void)
{
	INT8U _auch_Read[2] = {0x00};
	INT8U _auch_Write[2] ={0x12, 0x21};
	/*
	IapEeprom_Erase(0x0400);
	_auch_Read[0] = IapEeprom_Read(TEST_ADDR); //P0=0xff
	//发送
	Uart_SendString("Erase Read =", 12);
	Uart_SendByte(_auch_Read[0]);
	Uart_SendString("\r\n", 2);
	IapEeprom_Program(TEST_ADDR, _auch_Write[0]);
	_auch_Read[0] = IapEeprom_Read(TEST_ADDR);

	//发送	
	Uart_SendString("Program Read =", 14);
	Uart_SendByte(_auch_Read[0]);
	Uart_SendString("\r\n", 2);	*/
	//先读测试地址的数据
	_auch_Read[0] = IapEeprom_Read(TEST_ADDR);
	_auch_Read[1] = IapEeprom_Read(TEST_ADDR + 1);
	if((_auch_Read[0] == _auch_Write[0]) && (_auch_Read[1] == _auch_Write[1]))
	//读写一置认为EEPROM没坏
	{
		return;
	}
	else
	{
		//读写不一致，先擦除，再写
		IapEeprom_Erase(TEST_ADDR);
		IapEeprom_Erase(TEST_ADDR + 1);
		IapEeprom_Program(TEST_ADDR, _auch_Write[0]);//写数据进去
		IapEeprom_Program(TEST_ADDR + 1, _auch_Write[1]);
		if((_auch_Read[0] != _auch_Write[0]) && (_auch_Read[1] != _auch_Write[1]))
		//读写不一致
		{
			gst_SystemPara.uch_SystemStatus = ERROR;//系统错误
		}
	}
}	
/**
 *************************************************************************
 * @brief: SavePara_Eeprom
 * @param: _uch_Type -- 写入的参数类型
 * @return: TRUE -- 成功 FLASE -- 失败
 * @function:
 * @author: 
 * @version: V1.0
 * @note:
 *************************************************************************
**/
BOOL SavePara_Eeprom(INT8U _uch_Type)
{
	INT8U _auch_Addr[3] = {0x00};
	INT8U i;
	if(_uch_Type == REMOTECTL_ADDR)//保存类型为遥控器地址
	{
		gst_SystemPara.uch_RemoteCtlSavedNum++;//存储下一个遥控器地址
		gst_SystemPara.uch_RemoteCtlSavedNum %= MAX_REMOTE_NUM;//限制存储最大遥控数
		i = gst_SystemPara.uch_RemoteCtlSavedNum;
		
		_auch_Addr[0] = gst_RfRecData.ul_RemoteCtlAddr;
		_auch_Addr[1] = gst_RfRecData.ul_RemoteCtlAddr >> 8;
		_auch_Addr[2] = gst_RfRecData.ul_RemoteCtlAddr >> 16;
		//先擦除
		IapEeprom_Erase(PARA_ADDR + i * 3 + 1);
		IapEeprom_Erase(PARA_ADDR + i * 3 + 2);
		IapEeprom_Erase(PARA_ADDR + i * 3 + 3);
		//后写入
		IapEeprom_Program(PARA_ADDR + i * 3 + 1, _auch_Addr[0]);
		IapEeprom_Program(PARA_ADDR + i * 3 + 2, _auch_Addr[1]);
		IapEeprom_Program(PARA_ADDR + i * 3 + 3, _auch_Addr[2]);
		//读写一致，算写成功
		if((IapEeprom_Read(PARA_ADDR + i * 3 + 1) == _auch_Addr[0])
		&& (IapEeprom_Read(PARA_ADDR + i * 3 + 2) == _auch_Addr[1])
		&& (IapEeprom_Read(PARA_ADDR + i * 3 + 3) == _auch_Addr[2]))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else if(_uch_Type == REMOTECTL_TYPE)//遥控器控制类型
	{
		//先擦后写
		IapEeprom_Erase(PARA_ADDR + MAX_REMOTE_NUM * 3 + 1);
		IapEeprom_Program(PARA_ADDR + MAX_REMOTE_NUM * 3 + 1, gst_SystemPara.uch_RemoteCtlTrgType);
		//再读与写做判断
		if(IapEeprom_Read(PARA_ADDR + MAX_REMOTE_NUM * 3 + 1) == gst_SystemPara.uch_RemoteCtlTrgType)
		//参数写入成功
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}

}
/**
 *************************************************************************
 * @brief: ReadPara_Eeprom
 * @param: void
 * @return: void
 * @function:
 * @author: 
 * @version: V1.0
 * @note:
 *************************************************************************
**/
void ReadPara_Eeprom(void)
{
	INT8U _auch_Addr[3] = {0x00};
	INT8U i;	
	gst_SystemPara.uch_RemoteCtlSavedNum = IapEeprom_Read(PARA_ADDR);//从EEPROM读取系统参数
	gst_SystemPara.uch_RemoteCtlSavedNum %= MAX_REMOTE_NUM;//限制最大
	gst_SystemPara.uch_RemoteCtlTrgType = IapEeprom_Read(PARA_ADDR + MAX_REMOTE_NUM * 3 + 1);//遥控器输出模式
	gst_SystemPara.uch_RemoteCtlTrgType %= UART_OUTPUT + 1;//做参数有效检查


	for(i = 0; i < gst_SystemPara.uch_RemoteCtlSavedNum; i++)
	{
		_auch_Addr[0] = IapEeprom_Read(PARA_ADDR + i * 3 + 1);//从EEPROM读取系统参数
		_auch_Addr[1] = IapEeprom_Read(PARA_ADDR + i * 3 + 2);
		_auch_Addr[2] = IapEeprom_Read(PARA_ADDR + i * 3 + 3);
		gst_SystemPara.aul_RemoteCtlAddr[i] = ((INT32U)_auch_Addr[2] << 16) + ((INT32U)_auch_Addr[1] << 8) + _auch_Addr[0];
	}
}

