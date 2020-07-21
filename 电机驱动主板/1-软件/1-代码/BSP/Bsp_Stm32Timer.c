#include "Bsp_Stm32Timer.h"
#include "Mod_Spwm.h"

void Bsp_Stm32Timer_Init(void)
{
    PWM_TIMx_Init(gst_SpwmCtlPara.uin_TimerArr);	
}

void TIM4_Int_Init(u16 arr)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��ʹ��
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//TIM4ʱ��ʹ��    
   //���ø�����Ϊ�����������,���TIM3��CH3/CH4��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 |  GPIO_Pin_8; //
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	//��ʱ��TIM4��ʼ��
	TIM_TimeBaseStructure.TIM_Period = (arr - 1); //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler = 2 - 1; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE); //ʹ��ָ����TIM4�ж�,��������ж�
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ը�
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx

	TIM_Cmd(TIM4, ENABLE);//������ʱ��4
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00 ;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;		//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
}

///**
//  *************************************************************************
//  * @brief:TIM4_IRQHandler
//  * @param: void
//  * @return: void
//  * @author: 
//  * @version: V1.0
//  * @note:��ʱ��4�жϺ���
//  *************************************************************************
// **/ 
//void TIM4_IRQHandler(void)
//{ 	
//	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)//�Ǹ����ж�
//	{	 			   

//		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //���TIM4�����жϱ�־    

//	}	    
//}

void PWM_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);//ʹ��GPIOʱ��
    //�������������PA.8-->TIM1_CH1,PA.9-->TIM1_CH2,PA.10-->TIM1_CH2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;        //�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   
    GPIO_Init(GPIOA,&GPIO_InitStructure);                //ʹ��GPIOA
    //�������������PB.13-->TIM1_CH1N,PB.14-->TIM1_CH2N,PB.15-->TIM1_CH3N
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;        //�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   
    GPIO_Init(GPIOB,&GPIO_InitStructure);                //ʹ��GPIOB
	
	//BKIN-B12
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12;                           
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//GPIO_Mode_IPD;//
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void PWM_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	  
	/* Configure one bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* Enable the Update Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	  
	/* Enable the TIM1 BRK Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_BRK_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}
void PWM_TIMx_Init(INT16U _uin_Arr)
{
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    TIM_DeInit(TIM1);               //��ʱ��1��λ
	PWM_GPIO_Config();
    /*��ʱ��1��ʼ��*/
    TIM_TimeBaseStructure.TIM_Period = _uin_Arr;     //�Զ���װ��ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = 0;            //Ԥ��Ƶϵ��
    TIM_TimeBaseStructure.TIM_ClockDivision = 0X00;          //����ʱ�ӷָ�TDTS==tck_Tim
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;            //���ϼ���ģʽ
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    TIM_ARRPreloadConfig(TIM1, DISABLE);       //��ֹARRԤװ������
    /*˫·����PWM��ʼ��*/
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;          //TIM_OCMode_PWM1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;         //���������ʹ��
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;    //�����������ʹ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;                //PWM�������
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High;              //PWM��������
    TIM_OCInitStructure.TIM_Pulse = 0;                              //ռ�ձ�
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;              //���������״̬
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;        //�����󻥲��������״̬
    //��ʼ��3·������PWM��PA.8��PA.9��PA.10
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC3Init(TIM1, &TIM_OCInitStructure);
    //����3·������PWM
    //TIM_OC1PolarityConfig(TIM1,TIM_OCPreload_Enable);//PWMͨ��1Ԥװ��ʹ��
    //TIM_OC2PolarityConfig(TIM1,TIM_OCPreload_Enable);//PWMͨ��2Ԥװ��ʹ��
    //TIM_OC3PolarityConfig(TIM1,TIM_OCPreload_Enable);//PWMͨ��3Ԥװ��ʹ��
   
    //������ɲ�����ù��ܽ�����TIM1��TIM8�߼���ʱ����Ч
    TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;                //����ģʽ�µ����ʹ��
    TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;                //����ģʽ�µ����ʹ��
    TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;                    //��������
    TIM_BDTRInitStructure.TIM_DeadTime = DEAD_TIME;                                                //��������ʱ��2us
    TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;                            //ɲ�����ܹر�
    TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_Low;    //ɲ�����뼫��
    TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Disable;//�ر��Զ����ʹ��
    TIM_BDTRConfig(TIM1,&TIM_BDTRInitStructure);
    //�߼���ʱ�����������ʹ��ɲ��MOE�����ʹ��
    TIM_CtrlPWMOutputs(TIM1, ENABLE);        
    TIM_ARRPreloadConfig(TIM1, ENABLE);   
 
	PWM_NVIC_Config();
//	TIM_ClearITPendingBit(TIM1, TIM_IT_Break);
//    TIM_ITConfig(TIM1, TIM_IT_Break, ENABLE);

	// Clear Update Flag
	TIM_ClearFlag(TIM1, TIM_FLAG_Update);

	TIM_ITConfig(TIM1, TIM_IT_Update, DISABLE);
	
    //��ʱ����ʹ��   
    TIM_Cmd(TIM1, DISABLE); 
}

/*******************************************************************************
* Function Name  : TIM1_BRK_IRQHandler
* Description    : This function handles TIM1 Break interrupt request.
*******************************************************************************/
void TIM1_BRK_IRQHandler(void)
{
    //�ر�IGBT��������
    TIM_ClearITPendingBit(TIM1, TIM_IT_Break);
}

