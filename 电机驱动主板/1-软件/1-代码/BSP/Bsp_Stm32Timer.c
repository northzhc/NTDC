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
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟使能
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//TIM4时钟使能    
   //设置该引脚为复用输出功能,输出TIM3的CH3/CH4的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 |  GPIO_Pin_8; //
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	//定时器TIM4初始化
	TIM_TimeBaseStructure.TIM_Period = (arr - 1); //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler = 2 - 1; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE); //使能指定的TIM4中断,允许更新中断
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx

	TIM_Cmd(TIM4, ENABLE);//开启定时器4
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00 ;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;		//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
}

///**
//  *************************************************************************
//  * @brief:TIM4_IRQHandler
//  * @param: void
//  * @return: void
//  * @author: 
//  * @version: V1.0
//  * @note:定时器4中断函数
//  *************************************************************************
// **/ 
//void TIM4_IRQHandler(void)
//{ 	
//	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)//是更新中断
//	{	 			   

//		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //清除TIM4更新中断标志    

//	}	    
//}

void PWM_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);//使能GPIO时钟
    //正极性输出引脚PA.8-->TIM1_CH1,PA.9-->TIM1_CH2,PA.10-->TIM1_CH2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;        //复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   
    GPIO_Init(GPIOA,&GPIO_InitStructure);                //使能GPIOA
    //反极性输出引脚PB.13-->TIM1_CH1N,PB.14-->TIM1_CH2N,PB.15-->TIM1_CH3N
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;        //复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   
    GPIO_Init(GPIOB,&GPIO_InitStructure);                //使能GPIOB
	
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
    TIM_DeInit(TIM1);               //定时器1复位
	PWM_GPIO_Config();
    /*定时器1初始化*/
    TIM_TimeBaseStructure.TIM_Period = _uin_Arr;     //自动重装载值
    TIM_TimeBaseStructure.TIM_Prescaler = 0;            //预分频系数
    TIM_TimeBaseStructure.TIM_ClockDivision = 0X00;          //设置时钟分割TDTS==tck_Tim
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;            //向上计数模式
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    TIM_ARRPreloadConfig(TIM1, DISABLE);       //禁止ARR预装缓冲器
    /*双路互补PWM初始化*/
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;          //TIM_OCMode_PWM1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;         //正极性输出使能
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;    //互补极性输出使能
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;                //PWM输出极性
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High;              //PWM互补极性
    TIM_OCInitStructure.TIM_Pulse = 0;                              //占空比
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;              //死区后输出状态
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;        //死区后互补极性输出状态
    //初始化3路正极性PWM：PA.8，PA.9，PA.10
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC3Init(TIM1, &TIM_OCInitStructure);
    //配置3路正极性PWM
    //TIM_OC1PolarityConfig(TIM1,TIM_OCPreload_Enable);//PWM通道1预装载使能
    //TIM_OC2PolarityConfig(TIM1,TIM_OCPreload_Enable);//PWM通道2预装载使能
    //TIM_OC3PolarityConfig(TIM1,TIM_OCPreload_Enable);//PWM通道3预装载使能
   
    //死区和刹车配置功能仅仅对TIM1和TIM8高级定时器有效
    TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;                //空闲模式下的输出使能
    TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;                //运行模式下的输出使能
    TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;                    //锁定设置
    TIM_BDTRInitStructure.TIM_DeadTime = DEAD_TIME;                                                //设置死区时间2us
    TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;                            //刹车功能关闭
    TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_Low;    //刹车输入极性
    TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Disable;//关闭自动输出使能
    TIM_BDTRConfig(TIM1,&TIM_BDTRInitStructure);
    //高级定时器的输出控制使能刹车MOE主输出使能
    TIM_CtrlPWMOutputs(TIM1, ENABLE);        
    TIM_ARRPreloadConfig(TIM1, ENABLE);   
 
	PWM_NVIC_Config();
//	TIM_ClearITPendingBit(TIM1, TIM_IT_Break);
//    TIM_ITConfig(TIM1, TIM_IT_Break, ENABLE);

	// Clear Update Flag
	TIM_ClearFlag(TIM1, TIM_FLAG_Update);

	TIM_ITConfig(TIM1, TIM_IT_Update, DISABLE);
	
    //定时器不使能   
    TIM_Cmd(TIM1, DISABLE); 
}

/*******************************************************************************
* Function Name  : TIM1_BRK_IRQHandler
* Description    : This function handles TIM1 Break interrupt request.
*******************************************************************************/
void TIM1_BRK_IRQHandler(void)
{
    //关闭IGBT，并报错
    TIM_ClearITPendingBit(TIM1, TIM_IT_Break);
}

