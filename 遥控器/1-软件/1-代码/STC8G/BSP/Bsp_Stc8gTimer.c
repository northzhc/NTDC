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
	AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;
	TMOD |= 0x02; //ģʽ 2
	TH0 = TL0 = 0xEA; //24us
	TR0 = 1; //������ʱ��
	ET0 = 1; //ʹ�ܶ�ʱ���ж�
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
	RF433M_RecevieDecode();//����433���뺯��
	gst_SystemPara.ul_TimeCount++;//�˿ڸߵ�ƽ����ʱ�����++
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
	Timer0_Init();//��ʱ��0��ʼ��
}
