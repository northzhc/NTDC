#ifndef __GLOBAL_VALUE_H
#define __GLOBAL_VALUE_H

#include "STC8Gxx.h"
#include "Pub_TypeDefine.h"
/*�궨��*/
#define PARA_ADDR 0x5000//ϵͳ���������ַ
#define TEST_ADDR 0x4000//EEPROM�Լ��ַ

#define MAX_PORTHIGH_TIME 62500//�˿ڸߵ�ƽ����ʱ�� 24us * 62500 = 1.5s
#define MAX_CONFIGLAST_TIME 625000UL//����ʱ�� 24us * 625000 = 15s
#define MAX_BUTTONSCANF_TIME 500//24us * 500 = 12ms

#define MAX_REMOTE_NUM  4//���洢4��ң����ID

#define REMOTECTL_TYPE 0x11//ң��������
#define REMOTECTL_ADDR 0x22//��ַ
//�����˿ڶ���
#define  RF_KEY1_PIN P30
#define  RF_KEY2_PIN P31
#define  RF_KEY3_PIN P32
#define  RF_KEY4_PIN P33
//ң�������µ�״̬
typedef enum
{
	NO_PRESS,//û��
	LONG_PRESS,//����
	SHORT_PRESS,//�̰�
}RemoteCtlPressSta_E;
//ң�����������
typedef enum
{
	TURN_OVER,//��ת
	SLEF_LOCK,//����
	DOT_TRG,//�㶯
	INTER_LOCK,//����
	UART_OUTPUT,//�������
}RemoteCtlTrgType_E;
/*��������*/
typedef struct tag_SystemPara
{
	INT32U aul_RemoteCtlAddr[MAX_REMOTE_NUM];//ң������ַ
	INT8U  auch_DeviceID[8];//�豸ID��
	INT8U  uch_RemoteCtlSavedNum;//�Ѿ�¼�˵�ң����ID
	INT32U ul_TimeCount;//�˿ڸߵ�ƽ����ʱ�����
	INT8U  uch_RemoteCtlPressSta;//ң��������״̬

	INT8U  uch_SystemStatus;//ϵͳ״̬
	
	INT8U uch_RemoteCtlTrgType;//ң������������
}SystemPara_T;//ϵͳ����

typedef struct tag_RfFata
{
	INT32U ul_RemoteCtlAddr;//ң������ַ
	INT8U uch_KeyNum;//����ֵ
}RfData_T;//RF��������


typedef enum
{
	IDLE,//����
	RECV,//����ģʽ
	CONFIG,//����
	ERROR,//����
}SystemStatus_E;//ϵͳ״̬
/*��������*/
extern xdata RfData_T gst_RfRecData;
extern xdata SystemPara_T gst_SystemPara;


/*��������*/
extern void App_GlobalValue_Init(void);




#endif

