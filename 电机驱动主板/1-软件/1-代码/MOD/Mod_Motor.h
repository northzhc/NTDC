#ifndef __MOD_MOTOR_H
#define __MOD_MOTOR_H
#include "Pub_TypeDefine.h"

#define MAX_SPEED 15000//RPM
#define MIN_SPEED 100//RPM

#define MAX_FREQUENT  60//Hz
#define MIN_FREQUENT  5//Hz

#define MOTOR_POLE   2//电机的电级
enum MotorStatus_E
{
    IDLE = 0,	
    ROLL_RUN,
	ROLL_BACK,
	FAULT,
	SPEED_UP,
	SPEED_DOWN,
	OVER_CURRENT,
};


typedef struct tag_VFPara
{
	FP32 f_Voltage;//电压
	FP32 f_Frequent;//频率
	FP32 f_VFRatio;//电压频率比
}VFPara_T;
typedef struct tag_MotorCtrlPara
{
	INT16U uin_CurrentSpeed;///当前速度
	INT16U uin_TargetSpeed;//目标速度
	INT16U uch_Status;//状态
	
	INT8U uch_Direction;//方向
	
}MotorCtrlPara_T;//

extern MotorCtrlPara_T gst_MotorCtlPara;//
#endif

