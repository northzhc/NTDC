#ifndef __MOD_SPWM_H
#define __MOD_SPWM_H
#include "Pub_TypeDefine.h"
#include <math.h>
#define PI (4 * atan(1))//π
#define PULSE_ARRAY_NUM 1000//脉宽数据长度
/*SPWM参数范围*/
#define MAX_CARRIER  50000//50000HZ
#define MIN_CARRIER  1000//1000HZ
#define MAX_BASIC    5000//5000HZ
#define MIN_BASIC    5//5HZ
#define MAX_MODULATION 1
#define MIN_MODULATION 0.001
/*SPWM状态*/
#define SPWM_ON  0x11
#define SPWM_OFF 0x22
typedef struct tagSpwmPara
{
	INT16U auin_Pulse[PULSE_ARRAY_NUM];//比较寄存器
	INT16U uin_Length;//数组长度
	
	INT16U uin_UPlace;//U相
	INT16U uin_VPlace;//V相
	INT16U uin_WPlace;//W相
	
}SpwmPara_T;//SPWM参数
typedef struct tagSpwmCtlPara
{
	BOOL b_DataUseFlag;//数据使用标志
	BOOL b_DataChangeFlag;//数据改变标志
	BOOL b_PhaseChangeFlag;//换相标志
	INT16U uin_TimerArr;//定时器装载值
	
	FP32 f_Carrier;//载频
	FP32 f_Basic;//基频
	FP32 f_Modulation;//调制度


}SpwmCtlPara_T;//SPWM控制参数
extern SpwmPara_T gst_LastSpwmPara;
extern SpwmPara_T gst_NextSpwmPara;
extern SpwmCtlPara_T gst_SpwmCtlPara;


extern void Mod_Spwm_Init(void);
extern void Calculate_SpwmArray(FP32 _f_Basic, FP32 _f_Carrier, FP32 _f_Modulation);
extern void Control_Spwm(INT8U _uch_Status);
#endif

