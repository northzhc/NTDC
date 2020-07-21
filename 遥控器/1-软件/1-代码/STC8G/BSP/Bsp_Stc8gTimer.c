#include "Bsp_Stc8gTimer.h"
#include "Mod_Syn480r.h"
#include "App_GlobalValue.h"
/**
 *************************************************************************
 * @brief: 
 * @param: void
 * @return: void
 * @function:
 * @author: 
 * @version: V1.0
 * @note:
 *************************************************************************
**/
static void Timer0_Init(void)
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;
	TMOD |= 0x02; //模式 2
	TH0 = TL0 = 0xEA; //24us
	TR0 = 1; //启动定时器
	ET0 = 1; //使能定时器中断
	EA = 1;		
}
/**
 *************************************************************************
 * @brief: Timer0_Isr
 * @param: void
 * @return: void
 * @function:
 * @author: 
 * @version: V1.0
 * @note:
 *************************************************************************
**/
void Timer0_Isr() interrupt 1
{
	RF433M_RecevieDecode();//调用433解码函数
	gst_SystemPara.ul_TimeCount++;//端口高电平持续时间计数++
	//P33 =! P33;
}
/**
 *************************************************************************
 * @brief: Bsp_Stc8gTimer_Init
 * @param: void
 * @return: void
 * @function:
 * @author: 
 * @version: V1.0
 * @note:
 *************************************************************************
**/
void Bsp_Stc8gTimer_Init(void)
{
	Timer0_Init();//定时器0初始化
}
