#ifndef __PUB_TMY_FUNCTION_H
#define __PUB_TMY_FUNCTION_H
#ifdef __cplusplus
	extern "C" {
#endif
#include "Pub_MyFunction.h"
/*包含库的头文件*/
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include  "Pub_TypeDefine.h"

#define BIG_ENDIAN   0 //大端模式
#define SMALL_ENDAIN 1//小端模式
#define ENDIAN_MODE  SMALL_ENDAIN
#if(ENDIAN_MODE == BIG_ENDIAN)   //大端模式
typedef struct
{
    unsigned int byte0:8;
    unsigned int byte1:8;
}INT16U_T;

typedef struct
{
    signed int byte0:8;
    signed int byte1:8;
}INT16S_T;

typedef struct
{
    unsigned long byte0:8;
    unsigned long byte1:8;
    unsigned long byte2:8;
    unsigned long byte3:8;
}INT32U_T;

typedef struct
{
    signed long byte0:8;
    signed long byte1:8;
    signed long byte2:8;
    signed long byte3:8;
}INT32S_T;
#else    //小端模式
typedef struct
{
    unsigned int byte1:8;
    unsigned int byte0:8;
}INT16U_T;

typedef struct
{
   signed int byte1:8;
   signed int byte0:8;
}INT16S_T;

typedef struct
{
   unsigned long byte3:8;
   unsigned long byte2:8;
   unsigned long byte1:8;
   unsigned long byte0:8;
}INT32U_T;

typedef struct
{
   signed long byte3:8;
   signed long byte2:8;
   signed long byte1:8;
   signed long byte0:8;
}INT32S_T;
#endif

typedef union
{
    float          f_FloatData;
    signed long    lg_LongData;
    unsigned long  ul_UlongData;
    INT32U_T       st_bit;
}TYPECHANGE_N;
typedef union
{
    signed int     in_IntData;
    unsigned int   uin_UintData;
    INT16U_T       st_bit;
}TYPECHANGE2_N;

/*函数*/
void   EncodeFloat(FP32 _f_Data, INT8U* _puch_Buff);
FP32   DecodeFloat(INT8U* _puch_Buff);
void   EncodeUlong(INT32U _ul_Data, INT8U* _puch_Buff);
INT32U DecodeUlong(INT8U* _puch_Buff);
void   EncodeLong(INT32S _lg_Data, INT8U* _puch_Buff);
INT32S DecodeLong(INT8U* _puch_Buff);
void   EncodeUint(INT16U _uin_Data, INT8U* _puch_Buff);
INT16U DecodeUint(INT8U* _puch_Buff);
void   EncodeInt(INT16S _in_Data, INT8U* _puch_Buff);
INT16S DecodeInt(INT8U* _puch_Buff);
/*CRC16校验*/

/*冒泡排序*/
extern void BubbleSort(INT8U* _puch_Data, INT16U _uin_len);
extern INT16U GetCrc16Bit(INT8U* _puch_Data, INT16U _uin_CrcDataLen);

#ifdef __cplusplus
	}
#endif
#endif

