#include "Bsp_Stc8gClock.h"

void Bsp_Stc8gClock_Init(void)
{
	P_SW2 = 0x80;
	CKSEL = 0x00; //选择内部 IRC ( 默认 )
	HIRCCR = 1 << 7;//使能IRC
	while(!(HIRCCR & 0x01));//等待IRC稳定
	
	
	IRCBAND = 0x00;//20MHZ
	IRTRIM = 44;//24% * 44 
	LIRTRIM = 0x00;//微调频率 
	//系统时钟 22.1184MHZ
	

	CLKDIV = 0x02; //时钟2分频
	//2分频以后 11.0592MHz
	
	//查看系统时钟
	MCLKOCR = 0x01;//时钟输出到P5.4
	//MCLKOCR = 0x00;//不输出
	
	P_SW2 = 0x00;
}

