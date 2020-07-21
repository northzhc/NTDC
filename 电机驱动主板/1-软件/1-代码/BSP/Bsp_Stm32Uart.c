#include "Bsp_Stm32Uart.h"

int fputc(int ch, FILE *f)
{
	USART_SendData(USART2, (uint8_t) ch);

	while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET){}	
   
    return ch;
}

void Uart2_Init(INT32U _ul_baud)
{
  	//GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(UART2_GPIO_CLK, ENABLE); //GPIOA时钟
	RCC_APB1PeriphClockCmd(UART2_CLK, ENABLE);//使能USART2	
	
	//USART2_TX   GPIOA.2
    GPIO_InitStructure.GPIO_Pin = UART2_GPIO_TXPIN; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(UART2_GPIO_PORT, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART2_RX	  GPIOA.3初始化
    GPIO_InitStructure.GPIO_Pin = UART2_GPIO_RXPIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(UART2_GPIO_PORT, &GPIO_InitStructure);//初始化GPIOA.10  

  //Usart2 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00 ;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;		//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = _ul_baud;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

    USART_Init(USART2, &USART_InitStructure); //初始化串口1
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启串口接受中断
    USART_Cmd(USART2, ENABLE);                    //使能串口1 

}
void Bsp_Stm32Uart_Init(void)
{

	Uart2_Init(9600);
}
void USART2_IRQHandler(void) //串口2中断服务程序
{

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		USART_ReceiveData(USART2);	//读取接收到的数据
	}
}

