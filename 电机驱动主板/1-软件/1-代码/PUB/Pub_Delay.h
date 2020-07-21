#ifndef __PUB_DELAY_H
#define __PUB_DELAY_H
#include "stm32f10x.h"
#include "Pub_TypeDefine.h"

// 根据MCU做修改
#define MY_MCU_SYSCLK           SystemCoreClock


// 0xE000EDFC DEMCR RW Debug Exception and Monitor Control Register. 
#define DEMCR           (*(unsigned int *)0xE000EDFC ) 
#define TRCENA          (0x01 << 24) // DEMCR的DWT使能位   // 0xE0001000 DWT_CTRL RW The Debug Watchpoint and Trace (DWT) unit 
#define DWT_CTRL        (*(unsigned int *)0xE0001000 ) 
#define CYCCNTENA       (0x01 << 0 ) // DWT的SYCCNT使能位 // 0xE0001004 DWT_CYCCNT RW Cycle Count register,  
#define DWT_CYCCNT      (*(unsigned int *)0xE0001004) // 显示或设置处理器的周期计数值   
 

extern volatile INT32U gul_SystemTimerMs;
void DWT_Init(INT16U uin_sys_clk);
void DWT_Delay_Us(INT32U _ul_us); 
void Pub_SystemTick_Init(void);
void Delay_Us(INT32U _ul_us);
void Delay_Ms(INT32U _ul_ms);
void StartTickMs(INT32U *_pul_Ms);
INT32U GetTickMs(INT32U *_pul_Ms);
#endif


