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
	P3M0 = 0x00; //设置 P3.0~P3.7 为双向口模式
	P3M1 = 0x00;
	
	P5M0 = 0x00; //设置 P5.0~P5.7 为双向口模式
	P5M1 = 0x00;
	//P3M0 = 0xff; //设置 P3.0~P3.7 为开漏模式
	//P3M1 = 0xff
	P30 = P31 = P32 = P33 = 0;//初始为0
}


