#include "Bsp_Stm32Uart.h"

int fputc(int ch, FILE *f)
{
	USART_SendData(USART2, (uint8_t) ch);

	while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET){}	
   
    return ch;
}

void Uart2_Init(INT32U _ul_baud)
{
  	//GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(UART2_GPIO_CLK, ENABLE); //GPIOAʱ��
	RCC_APB1PeriphClockCmd(UART2_CLK, ENABLE);//ʹ��USART2	
	
	//USART2_TX   GPIOA.2
    GPIO_InitStructure.GPIO_Pin = UART2_GPIO_TXPIN; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(UART2_GPIO_PORT, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
  //USART2_RX	  GPIOA.3��ʼ��
    GPIO_InitStructure.GPIO_Pin = UART2_GPIO_RXPIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(UART2_GPIO_PORT, &GPIO_InitStructure);//��ʼ��GPIOA.10  

  //Usart2 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00 ;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;		//�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = _ul_baud;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART2, &USART_InitStructure); //��ʼ������1
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
    USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ���1 

}
void Bsp_Stm32Uart_Init(void)
{

	Uart2_Init(9600);
}
void USART2_IRQHandler(void) //����2�жϷ������
{

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		USART_ReceiveData(USART2);	//��ȡ���յ�������
	}
}

