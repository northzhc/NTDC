#include "Bsp_Stc8gEeprom.h"
#include "Bsp_Stc8gUart.h"
#include "App_GlobalValue.h"

void Bsp_Stc8gEeprom_Init(void)
{
	IapEeprom_Idle();//����	
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
	IAP_CONTR = 0; //�ر� IAP ����
	IAP_CMD = 0; //�������Ĵ���
	IAP_TRIG = 0; //��������Ĵ���
	IAP_ADDRH = 0x80; //����ַ���õ��� IAP ����
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
	IAP_CONTR = 0x80; //ʹ�� IAP
	IAP_TPS = 12; //���ò����ȴ����� 12MHz
	IAP_CMD = 1; //���� IAP ������
	IAP_ADDRL = _uin_Addr; //���� IAP �͵�ַ
	IAP_ADDRH = _uin_Addr >> 8; //���� IAP �ߵ�ַ
	IAP_TRIG = 0x5a; //д��������(0x5a)
	IAP_TRIG = 0xa5; //д��������(0xa5)
	_nop_();
	_uch_Data = IAP_DATA; //�� IAP ����
	IapEeprom_Idle(); //�ر� IAP ����
	return _uch_Data;
}
/**
 *************************************************************************
 * @brief: IapEeprom_Program
 * @param:  _uin_Addr -- ��ַ _uch_Data -- д�������
 * @return: void
 * @function:
 * @author: 
 * @version: V1.0
 * @note:
 *************************************************************************
**/
void IapEeprom_Program(INT16U _uin_Addr, INT8U _uch_Data)
{
	IAP_CONTR = 0x80; //ʹ�� IAP
	IAP_TPS = 12; //���ò����ȴ����� 12MHz
	IAP_CMD = 2; //���� IAP д����
	IAP_ADDRL = _uin_Addr; //���� IAP �͵�ַ
	IAP_ADDRH = _uin_Addr >> 8; //���� IAP �ߵ�ַ
	IAP_DATA = _uch_Data; //д IAP ����
	IAP_TRIG = 0x5a; //д��������(0x5a)
	IAP_TRIG = 0xa5; //д��������(0xa5)
	_nop_();
	IapEeprom_Idle(); //�ر� IAP ����
}
/**
 *************************************************************************
 * @brief: IapEeprom_Erase
 * @param: _uin_Addr -- ��ַ
 * @return: void
 * @function:
 * @author: 
 * @version: V1.0
 * @note:
 *************************************************************************
**/
void IapEeprom_Erase(INT16U _uin_Addr)
{
	IAP_CONTR = 0x80; //ʹ�� IAP
	IAP_TPS = 12; //���ò����ȴ����� 12MHz
	IAP_CMD = 3; //���� IAP ��������
	IAP_ADDRL = _uin_Addr; //���� IAP �͵�ַ
	IAP_ADDRH = _uin_Addr >> 8; //���� IAP �ߵ�ַ
	IAP_TRIG = 0x5a; //д��������(0x5a)
	IAP_TRIG = 0xa5; //д��������(0xa5)
	_nop_(); //
	IapEeprom_Idle(); //�ر� IAP ����
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
	//����
	Uart_SendString("Erase Read =", 12);
	Uart_SendByte(_auch_Read[0]);
	Uart_SendString("\r\n", 2);
	IapEeprom_Program(TEST_ADDR, _auch_Write[0]);
	_auch_Read[0] = IapEeprom_Read(TEST_ADDR);

	//����	
	Uart_SendString("Program Read =", 14);
	Uart_SendByte(_auch_Read[0]);
	Uart_SendString("\r\n", 2);	*/
	//�ȶ����Ե�ַ������
	_auch_Read[0] = IapEeprom_Read(TEST_ADDR);
	_auch_Read[1] = IapEeprom_Read(TEST_ADDR + 1);
	if((_auch_Read[0] == _auch_Write[0]) && (_auch_Read[1] == _auch_Write[1]))
	//��дһ����ΪEEPROMû��
	{
		return;
	}
	else
	{
		//��д��һ�£��Ȳ�������д
		IapEeprom_Erase(TEST_ADDR);
		IapEeprom_Erase(TEST_ADDR + 1);
		IapEeprom_Program(TEST_ADDR, _auch_Write[0]);//д���ݽ�ȥ
		IapEeprom_Program(TEST_ADDR + 1, _auch_Write[1]);
		if((_auch_Read[0] != _auch_Write[0]) && (_auch_Read[1] != _auch_Write[1]))
		//��д��һ��
		{
			gst_SystemPara.uch_SystemStatus = ERROR;//ϵͳ����
		}
	}
}	
/**
 *************************************************************************
 * @brief: SavePara_Eeprom
 * @param: _uch_Type -- д��Ĳ�������
 * @return: TRUE -- �ɹ� FLASE -- ʧ��
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
	if(_uch_Type == REMOTECTL_ADDR)//��������Ϊң������ַ
	{
		gst_SystemPara.uch_RemoteCtlSavedNum++;//�洢��һ��ң������ַ
		gst_SystemPara.uch_RemoteCtlSavedNum %= MAX_REMOTE_NUM;//���ƴ洢���ң����
		i = gst_SystemPara.uch_RemoteCtlSavedNum;
		
		_auch_Addr[0] = gst_RfRecData.ul_RemoteCtlAddr;
		_auch_Addr[1] = gst_RfRecData.ul_RemoteCtlAddr >> 8;
		_auch_Addr[2] = gst_RfRecData.ul_RemoteCtlAddr >> 16;
		//�Ȳ���
		IapEeprom_Erase(PARA_ADDR + i * 3 + 1);
		IapEeprom_Erase(PARA_ADDR + i * 3 + 2);
		IapEeprom_Erase(PARA_ADDR + i * 3 + 3);
		//��д��
		IapEeprom_Program(PARA_ADDR + i * 3 + 1, _auch_Addr[0]);
		IapEeprom_Program(PARA_ADDR + i * 3 + 2, _auch_Addr[1]);
		IapEeprom_Program(PARA_ADDR + i * 3 + 3, _auch_Addr[2]);
		//��дһ�£���д�ɹ�
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
	else if(_uch_Type == REMOTECTL_TYPE)//ң������������
	{
		//�Ȳ���д
		IapEeprom_Erase(PARA_ADDR + MAX_REMOTE_NUM * 3 + 1);
		IapEeprom_Program(PARA_ADDR + MAX_REMOTE_NUM * 3 + 1, gst_SystemPara.uch_RemoteCtlTrgType);
		//�ٶ���д���ж�
		if(IapEeprom_Read(PARA_ADDR + MAX_REMOTE_NUM * 3 + 1) == gst_SystemPara.uch_RemoteCtlTrgType)
		//����д��ɹ�
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
	gst_SystemPara.uch_RemoteCtlSavedNum = IapEeprom_Read(PARA_ADDR);//��EEPROM��ȡϵͳ����
	gst_SystemPara.uch_RemoteCtlSavedNum %= MAX_REMOTE_NUM;//�������
	gst_SystemPara.uch_RemoteCtlTrgType = IapEeprom_Read(PARA_ADDR + MAX_REMOTE_NUM * 3 + 1);//ң�������ģʽ
	gst_SystemPara.uch_RemoteCtlTrgType %= UART_OUTPUT + 1;//��������Ч���


	for(i = 0; i < gst_SystemPara.uch_RemoteCtlSavedNum; i++)
	{
		_auch_Addr[0] = IapEeprom_Read(PARA_ADDR + i * 3 + 1);//��EEPROM��ȡϵͳ����
		_auch_Addr[1] = IapEeprom_Read(PARA_ADDR + i * 3 + 2);
		_auch_Addr[2] = IapEeprom_Read(PARA_ADDR + i * 3 + 3);
		gst_SystemPara.aul_RemoteCtlAddr[i] = ((INT32U)_auch_Addr[2] << 16) + ((INT32U)_auch_Addr[1] << 8) + _auch_Addr[0];
	}
}

