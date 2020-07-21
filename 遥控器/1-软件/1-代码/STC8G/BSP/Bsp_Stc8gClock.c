#include "Bsp_Stc8gClock.h"

void Bsp_Stc8gClock_Init(void)
{
	P_SW2 = 0x80;
	CKSEL = 0x00; //ѡ���ڲ� IRC ( Ĭ�� )
	HIRCCR = 1 << 7;//ʹ��IRC
	while(!(HIRCCR & 0x01));//�ȴ�IRC�ȶ�
	
	
	IRCBAND = 0x00;//20MHZ
	IRTRIM = 44;//24% * 44 
	LIRTRIM = 0x00;//΢��Ƶ�� 
	//ϵͳʱ�� 22.1184MHZ
	

	CLKDIV = 0x02; //ʱ��2��Ƶ
	//2��Ƶ�Ժ� 11.0592MHz
	
	//�鿴ϵͳʱ��
	MCLKOCR = 0x01;//ʱ�������P5.4
	//MCLKOCR = 0x00;//�����
	
	P_SW2 = 0x00;
}

