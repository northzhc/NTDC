#include "App_GlobalValue.h"
#include "Bsp_Stc8gEeprom.h"
#include "string.h"

xdata RfData_T gst_RfRecData;//���ܵ�������

xdata SystemPara_T gst_SystemPara;//ϵͳ����


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
	gst_RfRecData.ul_RemoteCtlAddr = 0;//ң�ص�ַ
	gst_RfRecData.uch_KeyNum = 0;//����ֵ
	gst_SystemPara.uch_RemoteCtlSavedNum = 0;//�Ѿ�¼��ң����0��
	gst_SystemPara.uch_SystemStatus = IDLE;//����
	gst_SystemPara.ul_TimeCount = 0;//����ʱ����0
	gst_SystemPara.uch_RemoteCtlPressSta = NO_PRESS;//ûң������������
	gst_SystemPara.uch_RemoteCtlTrgType = DOT_TRG;//�㶯ģʽ
	ReadPara_Eeprom();//��ȡ����
}

