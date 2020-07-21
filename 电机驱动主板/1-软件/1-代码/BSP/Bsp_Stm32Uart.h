#ifndef __BSP_STM32USRT_H
#define __BSP_STM32USRT_H

#include "stm32f10x.h"
#include "Pub_TypeDefine.h"

#define UART2_CLK         RCC_APB1Periph_USART2
#define UART2_GPIO_CLK    RCC_APB2Periph_GPIOA
#define UART2_GPIO_PORT   GPIOA
#define UART2_GPIO_TXPIN  GPIO_Pin_2
#define UART2_GPIO_RXPIN  GPIO_Pin_3


void Bsp_Stm32Uart_Init(void);
#endif

