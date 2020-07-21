#include "Mod_Motor.h"
#include "Mod_Spwm.h"
MotorCtrlPara_T gst_MotorCtlPara;//���Ʋ���

//==================================================================================================
//| �������� | Motor_Process
//|---------|--------------------------------------------------------------------------------------
//| �������� | ���������
//|---------|--------------------------------------------------------------------------------------
//| ������� | ��
//|----------|--------------------------------------------------------------------------------------
//| ���ز��� | ��
//|---------|--------------------------------------------------------------------------------------
//| ������� | 2020/06/24
//==================================================================================================
void Motor_Process(void)
{
    switch(gst_MotorCtlPara.uch_Status)
    {
        case ROLL_RUN://��ת
        {
			Control_Spwm(SPWM_ON);//��SPWM���
        	gst_SpwmCtlPara.b_PhaseChangeFlag = TRUE;//��λ��־
            break;
        }
		case ROLL_BACK://��ת
		{
			Control_Spwm(SPWM_ON);//��SPWM���
        	gst_SpwmCtlPara.b_PhaseChangeFlag = FALSE;//��λ��־
            break;
		}
		case SPEED_UP://����
		{
			break;
		}
		case SPEED_DOWN://����
		{
			break;
		}
		case IDLE://����
		{
			Control_Spwm(SPWM_OFF);//�ر�SPWM���
			break;
		}
        default:
        break;
    }
}
void Motor_SpeedUp(void)
{
	
}
void Motor_SpeedDown(void)
{

}
void Motor_RollRun(void)
{
	
}
void Motor_RollBack(void)
{
	
}
INT16U Get_MotorSpeed(FP32 _f_Freq)
{

	return (INT16U)(_f_Freq * 60.0f / MOTOR_POLE);//

}

void Change_MotoFrequent(FP32 _f_Freq)
{
	FP32 _f_Modulation;

	if((_f_Freq > MAX_FREQUENT) || (_f_Freq < MIN_FREQUENT))
	{
		return;
	}
	//����Ƶ�ʼ���,VF�ȼ�����ƶ�
	//_f_Modulation
	Calculate_SpwmArray(_f_Freq, gst_SpwmCtlPara.f_Carrier, _f_Modulation);
}


