#include "Mod_Motor.h"
#include "Mod_Spwm.h"
MotorCtrlPara_T gst_MotorCtlPara;//控制参数

//==================================================================================================
//| 函数名称 | Motor_Process
//|---------|--------------------------------------------------------------------------------------
//| 函数功能 | 电机处理函数
//|---------|--------------------------------------------------------------------------------------
//| 输入参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | 无
//|---------|--------------------------------------------------------------------------------------
//| 函数设计 | 2020/06/24
//==================================================================================================
void Motor_Process(void)
{
    switch(gst_MotorCtlPara.uch_Status)
    {
        case ROLL_RUN://正转
        {
			Control_Spwm(SPWM_ON);//打开SPWM输出
        	gst_SpwmCtlPara.b_PhaseChangeFlag = TRUE;//相位标志
            break;
        }
		case ROLL_BACK://反转
		{
			Control_Spwm(SPWM_ON);//打开SPWM输出
        	gst_SpwmCtlPara.b_PhaseChangeFlag = FALSE;//相位标志
            break;
		}
		case SPEED_UP://加速
		{
			break;
		}
		case SPEED_DOWN://减速
		{
			break;
		}
		case IDLE://空闲
		{
			Control_Spwm(SPWM_OFF);//关闭SPWM输出
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
	//根据频率计算,VF比计算调制度
	//_f_Modulation
	Calculate_SpwmArray(_f_Freq, gst_SpwmCtlPara.f_Carrier, _f_Modulation);
}


