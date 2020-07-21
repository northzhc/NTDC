#ifndef __BSP_STC8GUART_H
#define __BSP_STC8GUART_H

#include "STC8Gxx.h"
#include "Pub_TypeDefine.h"

#define BRT (65536 - FOSC / 115200 / 4)


extern void Bsp_Stc8gUart_Init(void);
extern void Uart_SendByte(INT8U _uch_Byte);
extern void Uart_SendString(INT8U *_puch_Str, INT16U _uin_Len);
//extern void Uart_Printf(const char *fmt, ...);
extern INT8U Buff_Sum(INT8U *_puch_Data, INT16U _uin_Len);
#endif