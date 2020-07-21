#include "Bsp_Stm32Gpio.h"


void Bsp_Stm32Gpio_Init(void)
{
    //LED
    GPIO_InitTypeDef  GPIO_InitStructure;
    
	RCC_APB2PeriphClockCmd(LED_GPIO_CLK, ENABLE); //ʹ�ܶ˿�ʱ��	 
	GPIO_InitStructure.GPIO_Pin = LED_GPIO_PIN;// �˿����� 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(LED_GPIO_PORT, LED_GPIO_PIN);	//�����
	
}



