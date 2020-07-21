#include "App_Main.h"
INT32U ul_MainRunTime = 0;
INT16U i = 1;
int main(void)
{
    Bsp_Stm32Gpio_Init();
    Bsp_Stm32Uart_Init();
    Bsp_Stm32Timer_Init(); 
	Mod_Spwm_Init();
    Pub_SystemTick_Init();
	

    while(1)
	{
		if(GetTickMs(&ul_MainRunTime) >= 5000)
		{
			StartTickMs(&ul_MainRunTime);
			RUN_LED = !RUN_LED;	
	        
			switch(i)
			{
				case 1:
				{
					gst_SpwmCtlPara.b_PhaseChangeFlag =!gst_SpwmCtlPara.b_PhaseChangeFlag;
					Calculate_SpwmArray(50, 30000, 0.8);
					
					i = 2;
					break;
				}
				case 2:
				{
					gst_SpwmCtlPara.b_PhaseChangeFlag =!gst_SpwmCtlPara.b_PhaseChangeFlag;
					Calculate_SpwmArray(100, 30000, 0.8);
					i = 3;
					break;
				}
				case 3:
				{
					gst_SpwmCtlPara.b_PhaseChangeFlag =!gst_SpwmCtlPara.b_PhaseChangeFlag;
					Calculate_SpwmArray(200, 30000, 0.8);
					i = 4;
					break;
				}
				default:
				{
					i = 1;
					break;
				}
			}
			
			
		}
		//Delay_Ms(500);

	}
}
