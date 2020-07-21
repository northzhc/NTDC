#include "Mod_Button.h"
#include "App_GlobalValue.h"
#include "Bsp_Stc8gEeprom.h"
#include "Pub_Delay.h"

/**
 *************************************************************************
 * @brief: Mod_Button_Init
 * @param: void
 * @return: void
 * @function:
 * @author: 
 * @version: V1.0
 * @note:
 *************************************************************************
**/
void Mod_Button_Init(void)
{
	BUTTON_PIN = 1;//读之前置高
}
/**
 *************************************************************************
 * @brief: Button_Read
 * @param: void
 * @return: void
 * @function:
 * @author: 
 * @version: V1.0
 * @note:
 *************************************************************************
**/
static INT8U Button_Read(void)
{
	if(BUTTON_PIN == 0)
	{
		return BUTTON_PRESS;
	}
	else
	{
		return NO_PRESS;
	}
	
}
/**
 *************************************************************************
 * @brief: Button_Scanf
 * @param: void
 * @return: 按键值
 * @function:
 * @author: 
 * @version: V1.0
 * @note:
 *************************************************************************
**/
static INT8U Button_Scanf(void)
{
	static INT8U _uch_State = 0; //按键初始化状态
	static INT8U _uch_LastButton=0,_uch_NowButton=0; //记录两次电平的状态
	INT8U _uch_ButtonValue=0;
 
	_uch_NowButton = Button_Read();//读按键值
   
	switch(_uch_State)
	{
		case 0:
		{
			if(_uch_NowButton != _uch_LastButton)
			{				
				_uch_State = 1; //有按键按下
			}
			break;
		}
		case 1: 
		{
			if(_uch_NowButton == _uch_LastButton)
			{				
				_uch_State = 2; //消斗之后按键有效
			}
			else 
			{
				_uch_State = 0; //认为误触
			}
			break;
		} 
		case 2: //消斗之后
		{
			if(_uch_NowButton == _uch_LastButton) //还是按下的状态 
			{
			  	_uch_State = 3;
			}
			else//松开了，短按
			{
				_uch_State = 0; 				
				_uch_ButtonValue = _uch_LastButton | BUTTON_SHORT;  //返回键值短按	
			}
			break;
		}
		
		case 3: //判断长按短按
		{
			if(_uch_NowButton == _uch_LastButton) 
			{
			    static INT16U cnt = 0;
				if(cnt++ > 500) //500*10ms = 5s
				{
					cnt = 0; 
					_uch_State = 4;
					_uch_ButtonValue = _uch_LastButton | BUTTON_LONG; //返回键值长按
				}			  
			}
			else
			{
				_uch_State = 0;
				_uch_ButtonValue = _uch_LastButton | BUTTON_SHORT; //返回键值短按			
			}
			break;
		}
		case 4://长按松手检测
		{
			if(_uch_NowButton != _uch_LastButton) 
			{
				_uch_State = 0;
			}
			break;
		}
	}//switch
	
	_uch_LastButton = _uch_NowButton; //更新
	return _uch_ButtonValue;
}
/**
 *************************************************************************
 * @brief: Button_Process
 * @param: void
 * @return: void
 * @function:
 * @author: 
 * @version: V1.0
 * @note:
 *************************************************************************
**/
void Button_Process(void)
{
	INT8U _uch_ButtonNum;
	_uch_ButtonNum = Button_Scanf();//按键扫描
	if(_uch_ButtonNum == NO_BUTTON_PRESS)//没按键按下
	{
		return;
	}
	if(_uch_ButtonNum & BUTTON_SHORT)//短按
	{
		if(_uch_ButtonNum & BUTTON_PRESS)  
		{
			BUTTON_PIN = 0;//点亮LED灯
			gst_SystemPara.uch_RemoteCtlTrgType++;//切换模式
			gst_SystemPara.uch_RemoteCtlTrgType %= UART_OUTPUT + 1;//限制在翻转到串口输出之间
			if(!SavePara_Eeprom(REMOTECTL_TYPE))//保存参数失败
			{
				gst_SystemPara.uch_SystemStatus = ERROR;//错误模式
				return;
			}
			Delay_Ms(200);//短暂延时，看到灯熄灭切换成功
			BUTTON_PIN = 1;//关闭LED灯
		}	
	}
	else if(_uch_ButtonNum & BUTTON_LONG)//长按
	{
		if(_uch_ButtonNum & BUTTON_PRESS)  
		{
			BUTTON_PIN = 0;//点亮LED灯
			gst_RfRecData.ul_RemoteCtlAddr = 0;//清除接受到的遥控器ID
			//在进入配置模式后，再按下遥控器才能保存地址
			gst_SystemPara.uch_SystemStatus = CONFIG;//配置模式
			gst_SystemPara.ul_TimeCount = 0;//配置时长开始计数
			
		}		
	}
	
}
