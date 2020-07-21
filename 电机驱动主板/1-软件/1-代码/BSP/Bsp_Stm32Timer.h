#ifndef __BSP_STM32TIMER_H
#define __BSP_STM32TIMER_H

#include "stm32f10x.h"
#include "Pub_TypeDefine.h"
#define DEAD_TIME 0x7F
void Bsp_Stm32Timer_Init(void);
void PWM_TIMx_Init(INT16U _uin_Arr);

#endif
