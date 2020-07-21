#ifndef __MOD_MOTOR_H
#define __MOD_MOTOR_H
#include "Pub_TypeDefine.h"

#define MAX_SPEED 15000//RPM
#define MIN_SPEED 100//RPM

#define MAX_FREQUENT  60//Hz
#define MIN_FREQUENT  5//Hz

#define MOTOR_POLE   2//����ĵ缶
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
	FP32 f_Voltage;//��ѹ
	FP32 f_Frequent;//Ƶ��
	FP32 f_VFRatio;//��ѹƵ�ʱ�
}VFPara_T;
typedef struct tag_MotorCtrlPara
{
	INT16U uin_CurrentSpeed;///��ǰ�ٶ�
	INT16U uin_TargetSpeed;//Ŀ���ٶ�
	INT16U uch_Status;//״̬
	
	INT8U uch_Direction;//����
	
}MotorCtrlPara_T;//

extern MotorCtrlPara_T gst_MotorCtlPara;//
#endif

