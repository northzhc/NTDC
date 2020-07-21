#include "Bsp_Stc8gUart.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
static BOOL b_BusyFlag;
/**
 *************************************************************************
 * @brief: Bsp_Stc8gUart_Init
 * @param: void
 * @return: void
 * @function:
 * @author: 
 * @version: V1.0
 * @note: 115200 8位 无奇偶校验位
 *************************************************************************
**/
void Bsp_Stc8gUart_Init(void)
{
	//SCON = 0x50;		//8位数据,可变波特率
	TMOD &= 0x0F;
	TMOD |= 0x00;//定时器1模式0 为波特率发生器
	TL1 = BRT;
	TH1 = BRT >> 8;
	
	AUXR |= 0x40;//T1不12分频
	
	b_BusyFlag = FALSE;
	TR1 = 1;
	ES = 1;
	EA = 1;
	
	PS = 1;//串口优先级最高
	IPH = 1 << 4;
}
/**
 *************************************************************************
 * @brief: Uart_SendByte
 * @param: _uch_Byte -- 发送字节
 * @return: void
 * @function:
 * @author: 
 * @version: V1.0
 * @note:
 *************************************************************************
**/
void Uart_SendByte(INT8U _uch_Byte)
{
	while(b_BusyFlag);
	b_BusyFlag = TRUE;
	SBUF = _uch_Byte;	
}

/**
 *************************************************************************
 * @brief: Uart_SendString
 * @param: _puch_Str -- 要发送的字符串 _uin_Len -- 发送的长度
 * @return: void
 * @function:
 * @author: 
 * @version: V1.0
 * @note:
 *************************************************************************
**/
void Uart_SendString(INT8U *_puch_Str, INT16U _uin_Len) //reentrant //可重载关键字
{
//	//串口中断优先级置高 定时器为置低
//	PS = 1;
//	PT0 = 0;
//关闭定时器T0
	TR0 = 0;//发送数据的时候，不进行遥控接受
	while(_uin_Len--)
	{
		Uart_SendByte(*_puch_Str++);
	}
	TR0 = 1;//发送完成，打开定时器T0
	//串口中断优先级置低 定时器为置高
//	PS = 0;
//	PT0 = 1;
}
/**
 *************************************************************************
 * @brief: UartIsr
 * @param: void
 * @return: void
 * @function:
 * @author: 
 * @version: V1.0
 * @note:
 *************************************************************************
**/
void UartIsr() interrupt 4
{
	if (TI)
	{
		TI = 0;
		b_BusyFlag = FALSE;
	}
	if (RI)
	{
		RI = 0;
		//buffer[wptr++] = SBUF;
	}
}
/**
 *************************************************************************
 * @brief: Uart_Printf
 * @param: fmt -- 任意参数
 * @return: void
 * @function:
 * @author: 
 * @version: V1.0
 * @note:
 *************************************************************************
**/
//void Uart_Printf(const char *fmt, ...)
//{
//	va_list ap;
//	char xdata string[128];//访问内部拓展RAM，非访问外部RAM，不能超过内部拓展RAM大小(此处为1024)
//	
//	va_start(ap, fmt);
//	vsprintf(string, fmt, ap);//此处也可以使用sprintf函数，用法差不多，稍加修改即可，此处略去
//	Uart_SendString(string, strlen(string));
//	va_end(ap);
//}
/**
 *************************************************************************
 * @brief: Buff_Sum
 * @param: _puch_Data -- 求和的数据 _uin_Len -- 要求和的长度
 * @return: void
 * @function:
 * @author: 
 * @version: V1.0
 * @note:
 *************************************************************************
**/
INT8U Buff_Sum(INT8U *_puch_Data, INT16U _uin_Len)
{
	INT8U _uch_Sum = 0;
	INT8U i;
	for(i = 0; i < _uin_Len; i++)
	{
		_uch_Sum += *(_puch_Data + i);
	}
	return _uch_Sum;
}

