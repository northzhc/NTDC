#ifndef __GLOBAL_VALUE_H
#define __GLOBAL_VALUE_H

#include "STC8Gxx.h"
#include "Pub_TypeDefine.h"
/*宏定义*/
#define PARA_ADDR 0x5000//系统参数保存地址
#define TEST_ADDR 0x4000//EEPROM自检地址

#define MAX_PORTHIGH_TIME 62500//端口高电平持续时间 24us * 62500 = 1.5s
#define MAX_CONFIGLAST_TIME 625000UL//配置时长 24us * 625000 = 15s
#define MAX_BUTTONSCANF_TIME 500//24us * 500 = 12ms

#define MAX_REMOTE_NUM  4//最多存储4个遥控器ID

#define REMOTECTL_TYPE 0x11//遥控器类型
#define REMOTECTL_ADDR 0x22//地址
//按键端口定义
#define  RF_KEY1_PIN P30
#define  RF_KEY2_PIN P31
#define  RF_KEY3_PIN P32
#define  RF_KEY4_PIN P33
//遥控器按下的状态
typedef enum
{
	NO_PRESS,//没按
	LONG_PRESS,//长按
	SHORT_PRESS,//短按
}RemoteCtlPressSta_E;
//遥控器输出配置
typedef enum
{
	TURN_OVER,//翻转
	SLEF_LOCK,//自锁
	DOT_TRG,//点动
	INTER_LOCK,//互锁
	UART_OUTPUT,//串口输出
}RemoteCtlTrgType_E;
/*变量定义*/
typedef struct tag_SystemPara
{
	INT32U aul_RemoteCtlAddr[MAX_REMOTE_NUM];//遥控器地址
	INT8U  auch_DeviceID[8];//设备ID号
	INT8U  uch_RemoteCtlSavedNum;//已经录了的遥控器ID
	INT32U ul_TimeCount;//端口高电平持续时间计数
	INT8U  uch_RemoteCtlPressSta;//遥控器按键状态

	INT8U  uch_SystemStatus;//系统状态
	
	INT8U uch_RemoteCtlTrgType;//遥控器触发类型
}SystemPara_T;//系统参数

typedef struct tag_RfFata
{
	INT32U ul_RemoteCtlAddr;//遥控器地址
	INT8U uch_KeyNum;//按键值
}RfData_T;//RF接收数据


typedef enum
{
	IDLE,//空闲
	RECV,//接受模式
	CONFIG,//配置
	ERROR,//错误
}SystemStatus_E;//系统状态
/*变量声明*/
extern xdata RfData_T gst_RfRecData;
extern xdata SystemPara_T gst_SystemPara;


/*函数声明*/
extern void App_GlobalValue_Init(void);




#endif

