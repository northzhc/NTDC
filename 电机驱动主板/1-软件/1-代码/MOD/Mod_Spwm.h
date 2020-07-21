#ifndef __MOD_SPWM_H
#define __MOD_SPWM_H
#include "Pub_TypeDefine.h"
#include <math.h>
#define PI (4 * atan(1))//��
#define PULSE_ARRAY_NUM 1000//�������ݳ���
/*SPWM������Χ*/
#define MAX_CARRIER  50000//50000HZ
#define MIN_CARRIER  1000//1000HZ
#define MAX_BASIC    5000//5000HZ
#define MIN_BASIC    5//5HZ
#define MAX_MODULATION 1
#define MIN_MODULATION 0.001
/*SPWM״̬*/
#define SPWM_ON  0x11
#define SPWM_OFF 0x22
typedef struct tagSpwmPara
{
	INT16U auin_Pulse[PULSE_ARRAY_NUM];//�ȽϼĴ���
	INT16U uin_Length;//���鳤��
	
	INT16U uin_UPlace;//U��
	INT16U uin_VPlace;//V��
	INT16U uin_WPlace;//W��
	
}SpwmPara_T;//SPWM����
typedef struct tagSpwmCtlPara
{
	BOOL b_DataUseFlag;//����ʹ�ñ�־
	BOOL b_DataChangeFlag;//���ݸı��־
	BOOL b_PhaseChangeFlag;//�����־
	INT16U uin_TimerArr;//��ʱ��װ��ֵ
	
	FP32 f_Carrier;//��Ƶ
	FP32 f_Basic;//��Ƶ
	FP32 f_Modulation;//���ƶ�


}SpwmCtlPara_T;//SPWM���Ʋ���
extern SpwmPara_T gst_LastSpwmPara;
extern SpwmPara_T gst_NextSpwmPara;
extern SpwmCtlPara_T gst_SpwmCtlPara;


extern void Mod_Spwm_Init(void);
extern void Calculate_SpwmArray(FP32 _f_Basic, FP32 _f_Carrier, FP32 _f_Modulation);
extern void Control_Spwm(INT8U _uch_Status);
#endif

