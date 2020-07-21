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
 * @note: 115200 8λ ����żУ��λ
 *************************************************************************
**/
void Bsp_Stc8gUart_Init(void)
{
	//SCON = 0x50;		//8λ����,�ɱ䲨����
	TMOD &= 0x0F;
	TMOD |= 0x00;//��ʱ��1ģʽ0 Ϊ�����ʷ�����
	TL1 = BRT;
	TH1 = BRT >> 8;
	
	AUXR |= 0x40;//T1��12��Ƶ
	
	b_BusyFlag = FALSE;
	TR1 = 1;
	ES = 1;
	EA = 1;
	
	PS = 1;//�������ȼ����
	IPH = 1 << 4;
}
/**
 *************************************************************************
 * @brief: Uart_SendByte
 * @param: _uch_Byte -- �����ֽ�
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
 * @param: _puch_Str -- Ҫ���͵��ַ��� _uin_Len -- ���͵ĳ���
 * @return: void
 * @function:
 * @author: 
 * @version: V1.0
 * @note:
 *************************************************************************
**/
void Uart_SendString(INT8U *_puch_Str, INT16U _uin_Len) //reentrant //�����عؼ���
{
//	//�����ж����ȼ��ø� ��ʱ��Ϊ�õ�
//	PS = 1;
//	PT0 = 0;
//�رն�ʱ��T0
	TR0 = 0;//�������ݵ�ʱ�򣬲�����ң�ؽ���
	while(_uin_Len--)
	{
		Uart_SendByte(*_puch_Str++);
	}
	TR0 = 1;//������ɣ��򿪶�ʱ��T0
	//�����ж����ȼ��õ� ��ʱ��Ϊ�ø�
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
 * @param: fmt -- �������
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
//	char xdata string[128];//�����ڲ���չRAM���Ƿ����ⲿRAM�����ܳ����ڲ���չRAM��С(�˴�Ϊ1024)
//	
//	va_start(ap, fmt);
//	vsprintf(string, fmt, ap);//�˴�Ҳ����ʹ��sprintf�������÷���࣬�Լ��޸ļ��ɣ��˴���ȥ
//	Uart_SendString(string, strlen(string));
//	va_end(ap);
//}
/**
 *************************************************************************
 * @brief: Buff_Sum
 * @param: _puch_Data -- ��͵����� _uin_Len -- Ҫ��͵ĳ���
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

