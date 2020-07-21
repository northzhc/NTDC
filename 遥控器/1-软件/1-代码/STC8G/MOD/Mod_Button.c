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
	BUTTON_PIN = 1;//��֮ǰ�ø�
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
 * @return: ����ֵ
 * @function:
 * @author: 
 * @version: V1.0
 * @note:
 *************************************************************************
**/
static INT8U Button_Scanf(void)
{
	static INT8U _uch_State = 0; //������ʼ��״̬
	static INT8U _uch_LastButton=0,_uch_NowButton=0; //��¼���ε�ƽ��״̬
	INT8U _uch_ButtonValue=0;
 
	_uch_NowButton = Button_Read();//������ֵ
   
	switch(_uch_State)
	{
		case 0:
		{
			if(_uch_NowButton != _uch_LastButton)
			{				
				_uch_State = 1; //�а�������
			}
			break;
		}
		case 1: 
		{
			if(_uch_NowButton == _uch_LastButton)
			{				
				_uch_State = 2; //����֮�󰴼���Ч
			}
			else 
			{
				_uch_State = 0; //��Ϊ��
			}
			break;
		} 
		case 2: //����֮��
		{
			if(_uch_NowButton == _uch_LastButton) //���ǰ��µ�״̬ 
			{
			  	_uch_State = 3;
			}
			else//�ɿ��ˣ��̰�
			{
				_uch_State = 0; 				
				_uch_ButtonValue = _uch_LastButton | BUTTON_SHORT;  //���ؼ�ֵ�̰�	
			}
			break;
		}
		
		case 3: //�жϳ����̰�
		{
			if(_uch_NowButton == _uch_LastButton) 
			{
			    static INT16U cnt = 0;
				if(cnt++ > 500) //500*10ms = 5s
				{
					cnt = 0; 
					_uch_State = 4;
					_uch_ButtonValue = _uch_LastButton | BUTTON_LONG; //���ؼ�ֵ����
				}			  
			}
			else
			{
				_uch_State = 0;
				_uch_ButtonValue = _uch_LastButton | BUTTON_SHORT; //���ؼ�ֵ�̰�			
			}
			break;
		}
		case 4://�������ּ��
		{
			if(_uch_NowButton != _uch_LastButton) 
			{
				_uch_State = 0;
			}
			break;
		}
	}//switch
	
	_uch_LastButton = _uch_NowButton; //����
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
	_uch_ButtonNum = Button_Scanf();//����ɨ��
	if(_uch_ButtonNum == NO_BUTTON_PRESS)//û��������
	{
		return;
	}
	if(_uch_ButtonNum & BUTTON_SHORT)//�̰�
	{
		if(_uch_ButtonNum & BUTTON_PRESS)  
		{
			BUTTON_PIN = 0;//����LED��
			gst_SystemPara.uch_RemoteCtlTrgType++;//�л�ģʽ
			gst_SystemPara.uch_RemoteCtlTrgType %= UART_OUTPUT + 1;//�����ڷ�ת���������֮��
			if(!SavePara_Eeprom(REMOTECTL_TYPE))//�������ʧ��
			{
				gst_SystemPara.uch_SystemStatus = ERROR;//����ģʽ
				return;
			}
			Delay_Ms(200);//������ʱ��������Ϩ���л��ɹ�
			BUTTON_PIN = 1;//�ر�LED��
		}	
	}
	else if(_uch_ButtonNum & BUTTON_LONG)//����
	{
		if(_uch_ButtonNum & BUTTON_PRESS)  
		{
			BUTTON_PIN = 0;//����LED��
			gst_RfRecData.ul_RemoteCtlAddr = 0;//������ܵ���ң����ID
			//�ڽ�������ģʽ���ٰ���ң�������ܱ����ַ
			gst_SystemPara.uch_SystemStatus = CONFIG;//����ģʽ
			gst_SystemPara.ul_TimeCount = 0;//����ʱ����ʼ����
			
		}		
	}
	
}
