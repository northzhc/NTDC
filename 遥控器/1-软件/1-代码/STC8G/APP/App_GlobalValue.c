#include "App_GlobalValue.h"
#include "Bsp_Stc8gEeprom.h"
#include "string.h"

xdata RfData_T gst_RfRecData;//接受到的数据

xdata SystemPara_T gst_SystemPara;//系统参数


/**
 *************************************************************************
 * @brief: App_GlobalValue_Init
 * @param: void
 * @return: void
 * @function:
 * @author: 
 * @version: V1.0
 * @note:
 *************************************************************************
**/
void App_GlobalValue_Init(void)
{
	gst_RfRecData.ul_RemoteCtlAddr = 0;//遥控地址
	gst_RfRecData.uch_KeyNum = 0;//按键值
	gst_SystemPara.uch_RemoteCtlSavedNum = 0;//已经录了遥控器0个
	gst_SystemPara.uch_SystemStatus = IDLE;//空闲
	gst_SystemPara.ul_TimeCount = 0;//持续时间清0
	gst_SystemPara.uch_RemoteCtlPressSta = NO_PRESS;//没遥控器按键按下
	gst_SystemPara.uch_RemoteCtlTrgType = DOT_TRG;//点动模式
	ReadPara_Eeprom();//读取参数
}

