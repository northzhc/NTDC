#include "App_Main.h"

/*
--------------------|--------------------|-----------------------
 KEY/LED <<<< P5.4  |                    |P3.3 INT1  >>>>>输出1
--------------------|                    |-----------------------
	         VCC    |       SOP8         |P3.2 INT0  >>>>>输出2
--------------------|                    |-----------------------
    DATA <<<< P5.5  |     STC8G1K08      |P3.1 TXD   >>>>>输出3
--------------------|                    |-----------------------
	          GNG   |                    |P3.0 RXD   >>>>>输出4
--------------------|--------------------|-----------------------
*/

void main(void)
{	
	//Bsp_Stc8gClock_Init();//系统时钟初始化
	Bsp_Stc8gGpio_Init();//GPIO初始化
	Bsp_Stc8gTimer_Init();//定时器初始化
	Bsp_Stc8gUart_Init();//串口初始化
	Bsp_Stc8gEeprom_Init();//EEPROM初始化
	
	App_GlobalValue_Init();//全局变量初始化
	
	Mod_Syn480r_Init();//433接收初始化
	Mod_Button_Init();//按键初始化
	//Eeprom_Test();//EEPROM读写测试
	
	while(1)
	{
		
//		Uart_SendString("hello\r\n", 7);
		App_Process();//流程处理
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
 * @note: 空闲 -> 配置 -> 接收 -> 错误
 *************************************************************************
**/
void App_Process(void)
{
	switch(gst_SystemPara.uch_SystemStatus)//判断系统状态
	{
		case IDLE://空闲
		{
			if(gst_SystemPara.ul_TimeCount >= MAX_BUTTONSCANF_TIME)//到了按键扫描时间
			{
				gst_SystemPara.ul_TimeCount = 0;
				Button_Process();//按键处理
			}
			break;
		}
		case RECV://接受
		{
			RemoteCtl_Process();//遥控器接受处理
			if(gst_SystemPara.uch_RemoteCtlTrgType == DOT_TRG)//若为点动模式，大于1.5s后电平
			{
				if(gst_SystemPara.ul_TimeCount >= MAX_PORTHIGH_TIME)//大于高电平持续时间
				{
					RF_KEY1_PIN = RF_KEY2_PIN = RF_KEY3_PIN = RF_KEY4_PIN = 0;//清除端口状态		
					gst_RfRecData.ul_RemoteCtlAddr = 0;//清除接受到的遥控器ID
					gst_RfRecData.uch_KeyNum = 0;//清除接受到的按键值
					gst_SystemPara.uch_SystemStatus = IDLE;//切换到系统空闲
				}	
			}
			else
			{	
				gst_RfRecData.ul_RemoteCtlAddr = 0;//清除接受到的遥控器ID
				gst_RfRecData.uch_KeyNum = 0;//清除接受到的按键值
				gst_SystemPara.uch_SystemStatus = IDLE;//切换到系统空闲
			}
			break;
		}
		case CONFIG://配置
		{
			if(gst_RfRecData.ul_RemoteCtlAddr != 0)//有遥控器按下
			{
				if(SavePara_Eeprom(REMOTECTL_ADDR))//保存遥控器地址参数OK
				{
					BUTTON_PIN = 1;//配置成功 LED熄灭
					gst_SystemPara.uch_SystemStatus = IDLE;//系统空闲
				}
				else//保存失败
				{	
					gst_SystemPara.uch_SystemStatus = ERROR;//系统错误
				}
			}
			else
			{				
				BUTTON_PIN = !BUTTON_PIN;//配置模式 LED快闪
				Delay_Ms(100);
				if(gst_SystemPara.ul_TimeCount >= MAX_CONFIGLAST_TIME)//到了最长配置时间，退出配置模式
				{
					BUTTON_PIN = 1;//退出配置 LED熄灭
					gst_SystemPara.uch_SystemStatus = IDLE;//切换到系统空闲
				}

			}
			break;
		}
		case ERROR://错误
		{
			TR0 = 0;//关闭定时器0，不进行正常的解码
			BUTTON_PIN = 0;//LED常亮
			Uart_SendString("SYSTEM ERROR\r\n", 14);//串口输出系统出现错误
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
 * @function:遥控器接受处理函数
 * @author: 
 * @version: V1.0
 * @note: 按键按下端口状态保持1.5s
 *************************************************************************
**/
void RemoteCtl_Process(void)
{			

	INT8U i;
	switch(gst_SystemPara.uch_RemoteCtlPressSta)
	{
		case SHORT_PRESS://短按
		case LONG_PRESS://长按
		{
			//判断ADDR是否一样
			for(i = 0; i < gst_SystemPara.uch_RemoteCtlSavedNum; i++)
			{
				//判断接受到的地址是否与存储的地址相等
				if(gst_RfRecData.ul_RemoteCtlAddr == gst_SystemPara.aul_RemoteCtlAddr[i])
				{
					RemoteCtlOutput_Config();//根据设置类型输出
				}
			}
			gst_SystemPara.uch_RemoteCtlPressSta = NO_PRESS;//清除按下状态
			break;
		}
		case NO_PRESS://没按下
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
 * @function: 遥控器输出配置
 * @author: 
 * @version: V1.0
 * @note: 翻转 点动 互锁 自锁 串口 五种输出状态配置
 *************************************************************************
**/
void RemoteCtlOutput_Config(void)
{
	//判断设置的是哪种输出类型
	switch(gst_SystemPara.uch_RemoteCtlTrgType)
	{
		/*翻转 按下按键对应端口电平取反*/
		case TURN_OVER://翻转
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
		/*自锁 可任意组合键按下， 按下后对应的脚为高*/
		case SLEF_LOCK://自锁
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
				//两个按键按下
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
				//三个按键
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
				//四个按键按下
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
		/*点动 按下任意按键为高，保持1.5后为低*/
		case DOT_TRG://点动		
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
			gst_SystemPara.ul_TimeCount = 0;//从按键按下开始算起，1.5s保持对应的端口输出
			break;
		}
		/*互锁 每次只能有一个按键按下，这个按键为高，其他为低*/
		case INTER_LOCK://互锁
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
		case UART_OUTPUT://串口输出
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
