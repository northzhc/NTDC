//==================================================================================================
//| 文件名称 | Mod_Spwm.c
//|--------- |--------------------------------------------------------------------------------------
//| 文件描述 | spwm控制
//|--------- |--------------------------------------------------------------------------------------
//| 运行环境 | IAR or KEIL
//|--------- |--------------------------------------------------------------------------------------
//| 版权声明 |
//|----------|--------------------------------------------------------------------------------------
//| 包含文件 | App_Test.h
//|----------|--------------------------------------------------------------------------------------
//|  版本    |  时间       |  作者     | 描述
//|--------- |-------------|-----------|------------------------------------------------------------
//|  V1.0    | 2020-06-21  |           | 初版
//==================================================================================================

#include "Mod_Spwm.h"
#include <string.h>
#include "stm32f10x.h"

SpwmPara_T gst_LastSpwmPara;
SpwmPara_T gst_NextSpwmPara;

SpwmCtlPara_T gst_SpwmCtlPara;
static INT16U uin_CurrentLength = 0;//当前数据长度
static INT16U uin_CurrentUPlace = 0;//当前U相数据位置
static INT16U uin_CurrentVPlace = 0;//当前V相数据位置
static INT16U uin_CurrentWPlace = 0;//当前W相数据位置

//==================================================================================================
//| 函数名称 | Mod_Spwm_Init
//|---------|--------------------------------------------------------------------------------------
//| 函数功能 | SVPWM模块初始化
//|---------|--------------------------------------------------------------------------------------
//| 输入参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | 无
//|---------|--------------------------------------------------------------------------------------
//| 函数设计 | 2020/06/21 
//==================================================================================================

void Mod_Spwm_Init(void)
{
	gst_LastSpwmPara.uin_Length = 0;//数据长度
	gst_LastSpwmPara.uin_UPlace = 0;
	gst_LastSpwmPara.uin_VPlace = 0;
	gst_LastSpwmPara.uin_WPlace = 0;
	
	memset(gst_LastSpwmPara.auin_Pulse, 0, PULSE_ARRAY_NUM);
	gst_NextSpwmPara = gst_LastSpwmPara;

	gst_SpwmCtlPara.b_DataChangeFlag = FALSE;
	gst_SpwmCtlPara.uin_TimerArr = 0;
	gst_SpwmCtlPara.b_DataUseFlag = FALSE;
	Calculate_SpwmArray(50, 30000, 0.8);

	gst_SpwmCtlPara.b_DataUseFlag = TRUE;
	gst_SpwmCtlPara.b_DataChangeFlag = FALSE;
	gst_SpwmCtlPara.b_PhaseChangeFlag = FALSE;

	uin_CurrentLength = gst_LastSpwmPara.uin_Length;
	uin_CurrentVPlace = gst_LastSpwmPara.uin_UPlace;//
	uin_CurrentVPlace = gst_LastSpwmPara.uin_VPlace;//
	uin_CurrentWPlace = gst_LastSpwmPara.uin_WPlace;//

    Control_Spwm(SPWM_ON);
}

//==================================================================================================
//| 函数名称 | Cal_SpwmArray
//|---------|--------------------------------------------------------------------------------------
//| 函数功能 | SVPWM模块初始化
//|---------|--------------------------------------------------------------------------------------
//| 输入参数 | _f_Basic--基频 _f_Carrier--载频 _f_Modulation--调制度
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | 无
//|---------|--------------------------------------------------------------------------------------
//| 函数设计 | 2020/06/21
//==================================================================================================

void Calculate_SpwmArray(FP32 _f_Basic, FP32 _f_Carrier, FP32 _f_Modulation)
{
	FP64 _d_Td = 0;
	INT16U i;
	FP64 _d_SinA = 0;
	FP64 _d_SinB = 0;
	FP64 _d_SinC = 0;
	/*参数有效性判断*/
	if(((_f_Basic > MAX_BASIC) || (_f_Basic < MIN_BASIC))
	|| ((_f_Carrier > MAX_CARRIER) || (_f_Carrier < MIN_CARRIER))
	|| ((_f_Modulation > MAX_MODULATION) || (_f_Modulation < MIN_MODULATION)))
	{
		return;
	}
	
	
	gst_SpwmCtlPara.uin_TimerArr =  (INT16U)((FP32)SystemCoreClock / _f_Carrier) - 1;//APB2
	
	//gst_SpwmCtlPara.uin_TimerArr = (INT16U)(0.5 * (FP32)SystemCoreClock / _f_Carrier) - 1;//APB1	

	if(!gst_SpwmCtlPara.b_DataUseFlag)
	{
		gst_LastSpwmPara.uin_Length = (INT16U)(_f_Carrier / _f_Basic);//载频 / 基频
		if(!gst_SpwmCtlPara.b_PhaseChangeFlag)//换相
		{
			gst_LastSpwmPara.uin_UPlace = 0;
			gst_LastSpwmPara.uin_VPlace = (INT16U)(((FP32)gst_LastSpwmPara.uin_Length) / 3.0) - 1;//20
			gst_LastSpwmPara.uin_WPlace = (INT16U)(((((FP32)gst_LastSpwmPara.uin_Length) / 3.0)) * 2.0) - 1;//40
		}
		else
		{
			gst_LastSpwmPara.uin_UPlace = 0;
			gst_LastSpwmPara.uin_WPlace = (INT16U)(((FP32)gst_LastSpwmPara.uin_Length) / 3.0) - 1;//20
			gst_LastSpwmPara.uin_VPlace = (INT16U)(((((FP32)gst_LastSpwmPara.uin_Length) / 3.0)) * 2.0) - 1;//40		
		}
		for (i = 0; i < gst_LastSpwmPara.uin_Length; i++)
		{
			//20200716 del
			//gst_LastSpwmPara.auin_Pulse[i] = (INT16U)(0.5 * (1.0 + _f_Modulation * sin(2 * PI * _f_Basic * _d_Td)) * (FP32)(gst_SpwmCtlPara.uin_TimerArr + 1) + 0.5);
			//减少三次谐波分量，提高母线利用率
			_d_SinA = 1.15 * sin(2 * PI * _f_Basic * _d_Td);
			_d_SinB = 0.27 * sin(3 * 2 * PI * _f_Basic * _d_Td);
			_d_SinC = 0.029 * sin(9 * 2 * PI * _f_Basic * _d_Td);
			gst_NextSpwmPara.auin_Pulse[i] = (INT16U)(0.5 * (1.0 + _f_Modulation * (_d_SinA + _d_SinB + _d_SinC)) * (FP32)(gst_SpwmCtlPara.uin_TimerArr + 1) + 0.5);

			
			_d_Td += 1.0 / _f_Carrier;	
		}
	}
	else
	{
		gst_NextSpwmPara.uin_Length = (INT16U)(_f_Carrier / _f_Basic);//载频 / 基频
		if(!gst_SpwmCtlPara.b_PhaseChangeFlag)//换相
		{
			gst_NextSpwmPara.uin_UPlace = 0;
			gst_NextSpwmPara.uin_VPlace = (INT16U)(((FP32)gst_NextSpwmPara.uin_Length) / 3.0) - 1;//20
			gst_NextSpwmPara.uin_WPlace = (INT16U)(((((FP32)gst_NextSpwmPara.uin_Length) / 3.0)) * 2.0) - 1;//40
		}	
		else
		{
			gst_NextSpwmPara.uin_UPlace = 0;
			gst_NextSpwmPara.uin_WPlace = (INT16U)(((FP32)gst_NextSpwmPara.uin_Length) / 3.0) - 1;//20
			gst_NextSpwmPara.uin_VPlace = (INT16U)(((((FP32)gst_NextSpwmPara.uin_Length) / 3.0)) * 2.0) - 1;//40		
		}
		for (i = 0; i < gst_NextSpwmPara.uin_Length; i++)
		{
			//20200716 del
			//gst_NextSpwmPara.auin_Pulse[i] = (INT16U)(0.5 * (1.0 + _f_Modulation * sin(2 * PI * _f_Basic * _d_Td)) * (FP32)(gst_SpwmCtlPara.uin_TimerArr + 1) + 0.5);
			//减少三次谐波分量，提高母线利用率
			_d_SinA = 1.15 * sin(2 * PI * _f_Basic * _d_Td);
			_d_SinB = 0.27 * sin(3 * 2 * PI * _f_Basic * _d_Td);
			_d_SinC = 0.029 * sin(9 * 2 * PI * _f_Basic * _d_Td);
			gst_NextSpwmPara.auin_Pulse[i] = (INT16U)(0.5 * (1.0 + _f_Modulation * (_d_SinA + _d_SinB + _d_SinC)) * (FP32)(gst_SpwmCtlPara.uin_TimerArr + 1) + 0.5);
			_d_Td += 1.0 / _f_Carrier;	
		}
	}
	gst_SpwmCtlPara.b_DataChangeFlag = TRUE;
}
//==================================================================================================
//| 函数名称 | Generate_Spwm
//|---------|--------------------------------------------------------------------------------------
//| 函数功能 | 生成SPWM
//|---------|--------------------------------------------------------------------------------------
//| 输入参数 | _puin_CurrentNum--当前位置
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | 无
//|---------|--------------------------------------------------------------------------------------
//| 函数设计 | 2020/06/21
//==================================================================================================

static void Generate_Spwm(INT16U *_puin_CurrentNum)
{
	if(*_puin_CurrentNum == uin_CurrentLength)
	{
		*_puin_CurrentNum = 0; 
		if(gst_SpwmCtlPara.b_DataChangeFlag)//变频
		{
			
			gst_SpwmCtlPara.b_DataChangeFlag = FALSE;	
			gst_SpwmCtlPara.b_DataUseFlag = !gst_SpwmCtlPara.b_DataUseFlag;		

			if(gst_SpwmCtlPara.b_DataUseFlag)//换相
			{
				uin_CurrentLength = gst_LastSpwmPara.uin_Length;
				uin_CurrentUPlace = gst_LastSpwmPara.uin_UPlace;
				uin_CurrentVPlace = gst_LastSpwmPara.uin_VPlace;
				uin_CurrentWPlace = gst_LastSpwmPara.uin_WPlace; 
			}
			else
			{
				uin_CurrentLength = gst_NextSpwmPara.uin_Length;
				uin_CurrentUPlace = gst_NextSpwmPara.uin_UPlace;
				uin_CurrentVPlace = gst_NextSpwmPara.uin_VPlace;
				uin_CurrentWPlace = gst_NextSpwmPara.uin_WPlace; 				
			}    		
			TIM_SetAutoreload(TIM1, gst_SpwmCtlPara.uin_TimerArr);
		}
	}
	if(gst_SpwmCtlPara.b_DataUseFlag)//变频
	{
		TIM_SetCompare1(TIM1, gst_LastSpwmPara.auin_Pulse[(*_puin_CurrentNum + uin_CurrentUPlace) % uin_CurrentLength]);
		TIM_SetCompare2(TIM1, gst_LastSpwmPara.auin_Pulse[(*_puin_CurrentNum + uin_CurrentVPlace) % uin_CurrentLength]);
		TIM_SetCompare3(TIM1, gst_LastSpwmPara.auin_Pulse[(*_puin_CurrentNum + uin_CurrentWPlace) % uin_CurrentLength]);
		//printf("%d %d %d\r\n", TIM1->CCR1, TIM1->CCR2, TIM1->CCR3);
	}
	else
	{
		TIM_SetCompare1(TIM1, gst_NextSpwmPara.auin_Pulse[(*_puin_CurrentNum + uin_CurrentUPlace) % uin_CurrentLength]);
		TIM_SetCompare2(TIM1, gst_NextSpwmPara.auin_Pulse[(*_puin_CurrentNum + uin_CurrentVPlace) % uin_CurrentLength]);
		TIM_SetCompare3(TIM1, gst_NextSpwmPara.auin_Pulse[(*_puin_CurrentNum + uin_CurrentWPlace) % uin_CurrentLength]);
		//printf("%d %d %d\r\n", TIM1->CCR1, TIM1->CCR2, TIM1->CCR3);
	}	
}
//==================================================================================================
//| 函数名称 | TIM1_UP_IRQHandler
//|---------|--------------------------------------------------------------------------------------
//| 函数功能 | TIM1更新中断服务函数
//|---------|--------------------------------------------------------------------------------------
//| 输入参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | 无
//|---------|--------------------------------------------------------------------------------------
//| 函数设计 | 2020/06/21
//==================================================================================================

void TIM1_UP_IRQHandler(void)
{
	static INT16U i = 0;
	if(TIM_GetITStatus(TIM1,TIM_IT_Update) != RESET)
	{
		i++;
		Generate_Spwm(&i);//生成SPWM
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);//清除中断标志
	}
}
//==================================================================================================
//| 函数名称 | Control_Spwm
//|---------|--------------------------------------------------------------------------------------
//| 函数功能 | SPWM控制
//|---------|--------------------------------------------------------------------------------------
//| 输入参数 | _uch_Status -- 状态  SPWM_ON打开 SPWM_OFF关闭
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | 无
//|---------|--------------------------------------------------------------------------------------
//| 函数设计 | 2020/06/21 
//==================================================================================================

void Control_Spwm(INT8U _uch_Status)
{
	switch(_uch_Status)
	{
		case SPWM_ON://打开
		{
			TIM_ClearITPendingBit(TIM1, TIM_IT_Break);
			TIM_ClearFlag(TIM1, TIM_FLAG_Update);
            TIM_CtrlPWMOutputs(TIM1, ENABLE);
			TIM_Cmd(TIM1, ENABLE);	
			break;
		}
		case SPWM_OFF://关闭
		{
			TIM_Cmd(TIM1, DISABLE);
            TIM_CtrlPWMOutputs(TIM1, DISABLE);
			TIM_ClearITPendingBit(TIM1, TIM_IT_Break);
			TIM_ClearFlag(TIM1, TIM_FLAG_Update);
			break;
		}
		default:
			break;
	}
}

//==================================================================================================
//| 函数名称 | TIM1_BRK_IRQHandler
//|---------|--------------------------------------------------------------------------------------
//| 函数功能 | TIM1中断服务函数
//|---------|--------------------------------------------------------------------------------------
//| 输入参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | 无
//|---------|--------------------------------------------------------------------------------------
//| 函数设计 | 2020/06/21 
//==================================================================================================

//void TIM1_BRK_IRQHandler(void)
//{
//	TIM_CtrlPWMOutputs(TIM1, DISABLE);
//    TIM_ITConfig(TIM1, TIM_IT_Update,DISABLE);
//	TIM_Cmd(TIM1, DISABLE);
//}


