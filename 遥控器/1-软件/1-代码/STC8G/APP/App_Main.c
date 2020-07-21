#include "App_Main.h"

/*
--------------------|--------------------|-----------------------
 KEY/LED <<<< P5.4  |                    |P3.3 INT1  >>>>>���1
--------------------|                    |-----------------------
	         VCC    |       SOP8         |P3.2 INT0  >>>>>���2
--------------------|                    |-----------------------
    DATA <<<< P5.5  |     STC8G1K08      |P3.1 TXD   >>>>>���3
--------------------|                    |-----------------------
	          GNG   |                    |P3.0 RXD   >>>>>���4
--------------------|--------------------|-----------------------
*/

void main(void)
{	
	//Bsp_Stc8gClock_Init();//ϵͳʱ�ӳ�ʼ��
	Bsp_Stc8gGpio_Init();//GPIO��ʼ��
	Bsp_Stc8gTimer_Init();//��ʱ����ʼ��
	Bsp_Stc8gUart_Init();//���ڳ�ʼ��
	Bsp_Stc8gEeprom_Init();//EEPROM��ʼ��
	
	App_GlobalValue_Init();//ȫ�ֱ�����ʼ��
	
	Mod_Syn480r_Init();//433���ճ�ʼ��
	Mod_Button_Init();//������ʼ��
	//Eeprom_Test();//EEPROM��д����
	
	while(1)
	{
		
//		Uart_SendString("hello\r\n", 7);
		App_Process();//���̴���
	}
}
/**
 *************************************************************************
 * @brief: App_Process
 * @param: void
 * @return: void
 * @function:
 * @author: 
 * @version: V1.0
 * @note: ���� -> ���� -> ���� -> ����
 *************************************************************************
**/
void App_Process(void)
{
	switch(gst_SystemPara.uch_SystemStatus)//�ж�ϵͳ״̬
	{
		case IDLE://����
		{
			if(gst_SystemPara.ul_TimeCount >= MAX_BUTTONSCANF_TIME)//���˰���ɨ��ʱ��
			{
				gst_SystemPara.ul_TimeCount = 0;
				Button_Process();//��������
			}
			break;
		}
		case RECV://����
		{
			RemoteCtl_Process();//ң�������ܴ���
			if(gst_SystemPara.uch_RemoteCtlTrgType == DOT_TRG)//��Ϊ�㶯ģʽ������1.5s���ƽ
			{
				if(gst_SystemPara.ul_TimeCount >= MAX_PORTHIGH_TIME)//���ڸߵ�ƽ����ʱ��
				{
					RF_KEY1_PIN = RF_KEY2_PIN = RF_KEY3_PIN = RF_KEY4_PIN = 0;//����˿�״̬		
					gst_RfRecData.ul_RemoteCtlAddr = 0;//������ܵ���ң����ID
					gst_RfRecData.uch_KeyNum = 0;//������ܵ��İ���ֵ
					gst_SystemPara.uch_SystemStatus = IDLE;//�л���ϵͳ����
				}	
			}
			else
			{	
				gst_RfRecData.ul_RemoteCtlAddr = 0;//������ܵ���ң����ID
				gst_RfRecData.uch_KeyNum = 0;//������ܵ��İ���ֵ
				gst_SystemPara.uch_SystemStatus = IDLE;//�л���ϵͳ����
			}
			break;
		}
		case CONFIG://����
		{
			if(gst_RfRecData.ul_RemoteCtlAddr != 0)//��ң��������
			{
				if(SavePara_Eeprom(REMOTECTL_ADDR))//����ң������ַ����OK
				{
					BUTTON_PIN = 1;//���óɹ� LEDϨ��
					gst_SystemPara.uch_SystemStatus = IDLE;//ϵͳ����
				}
				else//����ʧ��
				{	
					gst_SystemPara.uch_SystemStatus = ERROR;//ϵͳ����
				}
			}
			else
			{				
				BUTTON_PIN = !BUTTON_PIN;//����ģʽ LED����
				Delay_Ms(100);
				if(gst_SystemPara.ul_TimeCount >= MAX_CONFIGLAST_TIME)//���������ʱ�䣬�˳�����ģʽ
				{
					BUTTON_PIN = 1;//�˳����� LEDϨ��
					gst_SystemPara.uch_SystemStatus = IDLE;//�л���ϵͳ����
				}

			}
			break;
		}
		case ERROR://����
		{
			TR0 = 0;//�رն�ʱ��0�������������Ľ���
			BUTTON_PIN = 0;//LED����
			Uart_SendString("SYSTEM ERROR\r\n", 14);//�������ϵͳ���ִ���
			break;
		}
		default:
			break;
	}
}
/**
 *************************************************************************
 * @brief: RemoteCtl_Process
 * @param: void
 * @return: void
 * @function:ң�������ܴ�����
 * @author: 
 * @version: V1.0
 * @note: �������¶˿�״̬����1.5s
 *************************************************************************
**/
void RemoteCtl_Process(void)
{			

	INT8U i;
	switch(gst_SystemPara.uch_RemoteCtlPressSta)
	{
		case SHORT_PRESS://�̰�
		case LONG_PRESS://����
		{
			//�ж�ADDR�Ƿ�һ��
			for(i = 0; i < gst_SystemPara.uch_RemoteCtlSavedNum; i++)
			{
				//�жϽ��ܵ��ĵ�ַ�Ƿ���洢�ĵ�ַ���
				if(gst_RfRecData.ul_RemoteCtlAddr == gst_SystemPara.aul_RemoteCtlAddr[i])
				{
					RemoteCtlOutput_Config();//���������������
				}
			}
			gst_SystemPara.uch_RemoteCtlPressSta = NO_PRESS;//�������״̬
			break;
		}
		case NO_PRESS://û����
		{
			break;
		}
		default:
			break;
	}
}
/**
 *************************************************************************
 * @brief: RemoteCtlOutput_Config
 * @param: void
 * @return: void
 * @function: ң�����������
 * @author: 
 * @version: V1.0
 * @note: ��ת �㶯 ���� ���� ���� �������״̬����
 *************************************************************************
**/
void RemoteCtlOutput_Config(void)
{
	//�ж����õ��������������
	switch(gst_SystemPara.uch_RemoteCtlTrgType)
	{
		/*��ת ���°�����Ӧ�˿ڵ�ƽȡ��*/
		case TURN_OVER://��ת
		{
			switch(gst_RfRecData.uch_KeyNum)
			{
				case C_RF_DATA_1_KEY: 
				{ 
					RF_KEY1_PIN = !RF_KEY1_PIN;
					break; 
				} 
				case C_RF_DATA_2_KEY: 
				{  
					RF_KEY2_PIN = !RF_KEY2_PIN;
					break;
				} 
				case C_RF_DATA_3_KEY: 
				{	  
					RF_KEY3_PIN = !RF_KEY3_PIN;
					break; 
				}
				case C_RF_DATA_4_KEY:
				{
					RF_KEY4_PIN = !RF_KEY4_PIN;
					break; 
				}
				default:
				{
					RF_KEY1_PIN = RF_KEY2_PIN = RF_KEY3_PIN = RF_KEY4_PIN = 0;
					break;
				}
			}
			break;
		}
		/*���� ��������ϼ����£� ���º��Ӧ�Ľ�Ϊ��*/
		case SLEF_LOCK://����
		{
			switch(gst_RfRecData.uch_KeyNum)
			{
				case C_RF_DATA_1_KEY: 
				{ 
					RF_KEY1_PIN = 1;
					RF_KEY2_PIN = RF_KEY3_PIN = RF_KEY4_PIN = 0;
					break; 
				} 
				case C_RF_DATA_2_KEY: 
				{  
					RF_KEY2_PIN = 1;
					RF_KEY1_PIN = RF_KEY3_PIN = RF_KEY4_PIN = 0;
					break;
				} 
				case C_RF_DATA_3_KEY: 
				{	  
					RF_KEY3_PIN = 1;
					RF_KEY1_PIN = RF_KEY2_PIN = RF_KEY4_PIN = 0;
					break; 
				}
				case C_RF_DATA_4_KEY:
				{
					RF_KEY4_PIN = 1;
					RF_KEY1_PIN = RF_KEY2_PIN = RF_KEY3_PIN = 0;
					break; 
				}
				//������������
				case C_RF_DATA_1_KEY | C_RF_DATA_2_KEY:
				{
					RF_KEY1_PIN = RF_KEY2_PIN = 1;
					RF_KEY3_PIN = RF_KEY4_PIN = 0;
					break;
				}
				case C_RF_DATA_1_KEY | C_RF_DATA_3_KEY:
				{
					RF_KEY1_PIN = RF_KEY3_PIN = 1;
					RF_KEY2_PIN = RF_KEY4_PIN = 0;
					break;
				}
				case C_RF_DATA_1_KEY | C_RF_DATA_4_KEY:
				{
					RF_KEY1_PIN = RF_KEY4_PIN = 1;
					RF_KEY2_PIN = RF_KEY3_PIN = 0;
					break;
				}
				case C_RF_DATA_2_KEY | C_RF_DATA_3_KEY:
				{
					RF_KEY2_PIN = RF_KEY3_PIN = 1;
					RF_KEY1_PIN = RF_KEY4_PIN = 0;
					break;
				}
				case C_RF_DATA_2_KEY | C_RF_DATA_4_KEY:
				{
					RF_KEY2_PIN = RF_KEY4_PIN = 1;
					RF_KEY1_PIN = RF_KEY3_PIN = 0;
					break;
				}
				case C_RF_DATA_3_KEY | C_RF_DATA_4_KEY:
				{
					RF_KEY3_PIN = RF_KEY4_PIN = 1;
					RF_KEY1_PIN = RF_KEY2_PIN = 0;
					break;
				}
				//��������
				case C_RF_DATA_1_KEY | C_RF_DATA_2_KEY | C_RF_DATA_3_KEY:
				{
					RF_KEY1_PIN = RF_KEY2_PIN = RF_KEY3_PIN = 1;
					RF_KEY4_PIN = 0;
					break;
				}
				case C_RF_DATA_1_KEY | C_RF_DATA_2_KEY | C_RF_DATA_4_KEY:
				{
					RF_KEY1_PIN = RF_KEY2_PIN = RF_KEY4_PIN = 1;
					RF_KEY3_PIN = 0;
					break;
				}
				case C_RF_DATA_1_KEY | C_RF_DATA_3_KEY | C_RF_DATA_4_KEY:
				{
					RF_KEY1_PIN = RF_KEY3_PIN = RF_KEY4_PIN = 1;
					RF_KEY2_PIN = 0;
					break;
				}
				//�ĸ���������
				case C_RF_DATA_1_KEY | C_RF_DATA_2_KEY | C_RF_DATA_3_KEY | C_RF_DATA_4_KEY:
				{
					RF_KEY1_PIN = RF_KEY2_PIN = RF_KEY3_PIN = RF_KEY4_PIN = 1;
					break;
				}				
				default:
				{
					RF_KEY1_PIN = RF_KEY2_PIN = RF_KEY3_PIN = RF_KEY4_PIN = 0;
					break;
				}
			}
			break;
		}
		/*�㶯 �������ⰴ��Ϊ�ߣ�����1.5��Ϊ��*/
		case DOT_TRG://�㶯		
		{
			switch(gst_RfRecData.uch_KeyNum)
			{
				case C_RF_DATA_1_KEY: 
				{ 
					RF_KEY1_PIN = 1;
					break; 
				} 
				case C_RF_DATA_2_KEY: 
				{  
					RF_KEY2_PIN = 1;
					break;
				} 
				case C_RF_DATA_3_KEY: 
				{	  
					RF_KEY3_PIN = 1;
					break; 
				}
				case C_RF_DATA_4_KEY:
				{
					RF_KEY4_PIN = 1;
					break; 
				}
				default:
				{
					RF_KEY1_PIN = RF_KEY2_PIN = RF_KEY3_PIN = RF_KEY4_PIN = 0;
					break;
				}
			}		
			gst_SystemPara.ul_TimeCount = 0;//�Ӱ������¿�ʼ����1.5s���ֶ�Ӧ�Ķ˿����
			break;
		}
		/*���� ÿ��ֻ����һ���������£��������Ϊ�ߣ�����Ϊ��*/
		case INTER_LOCK://����
		{	
			switch(gst_RfRecData.uch_KeyNum)
			{
				case C_RF_DATA_1_KEY: 
				{ 
					RF_KEY1_PIN = 1;
					RF_KEY2_PIN = RF_KEY3_PIN = RF_KEY4_PIN = 0;
					break; 
				} 
				case C_RF_DATA_2_KEY: 
				{  
					RF_KEY2_PIN = 1;
					RF_KEY1_PIN = RF_KEY3_PIN = RF_KEY4_PIN = 0;
					break;
				} 
				case C_RF_DATA_3_KEY: 
				{	  
					RF_KEY3_PIN = 1;
					RF_KEY1_PIN = RF_KEY2_PIN = RF_KEY4_PIN = 0;
					break; 
				}
				case C_RF_DATA_4_KEY:
				{
					RF_KEY4_PIN = 1;
					RF_KEY1_PIN = RF_KEY2_PIN = RF_KEY3_PIN = 0;
					break; 
				}
				default:
				{
					RF_KEY1_PIN = RF_KEY2_PIN = RF_KEY3_PIN = RF_KEY4_PIN = 0;
					break;
				}
			}
			
			break;
		}
		case UART_OUTPUT://�������
		{
			INT8U auch_Buff[7]={0x7B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7D};
			auch_Buff[1] = gst_RfRecData.ul_RemoteCtlAddr >> 16;
			auch_Buff[2] = gst_RfRecData.ul_RemoteCtlAddr >> 8;
			auch_Buff[3] = gst_RfRecData.ul_RemoteCtlAddr;
			auch_Buff[4] = gst_RfRecData.uch_KeyNum;
			auch_Buff[5] = Buff_Sum(auch_Buff, 5);
			Uart_SendString(auch_Buff, sizeof(auch_Buff) / sizeof(auch_Buff[0]));
			break;
		}	
		default:
			break;
	}
}
