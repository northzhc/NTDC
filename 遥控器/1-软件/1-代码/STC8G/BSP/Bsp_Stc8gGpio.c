#include "Bsp_Stc8gGpio.h"
/**
 *************************************************************************
 * @brief: Bsp_Stc8gGpio_Init
 * @param: void
 * @return: void
 * @function:
 * @author: 
 * @version: V1.0
 * @note:
 *************************************************************************
**/
void Bsp_Stc8gGpio_Init(void)
{
	P3M0 = 0x00; //���� P3.0~P3.7 Ϊ˫���ģʽ
	P3M1 = 0x00;
	
	P5M0 = 0x00; //���� P5.0~P5.7 Ϊ˫���ģʽ
	P5M1 = 0x00;
	//P3M0 = 0xff; //���� P3.0~P3.7 Ϊ��©ģʽ
	//P3M1 = 0xff
	P30 = P31 = P32 = P33 = 0;//��ʼΪ0
}


