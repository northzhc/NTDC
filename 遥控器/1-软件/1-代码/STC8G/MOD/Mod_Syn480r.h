#ifndef __MOD_SYN480R_H
#define __MOD_SYN480R_H

#include "STC8Gxx.h"
#include "Pub_TypeDefine.h"

/***************************************************/
 
//Define I/O Register
#define RF_PIN  P54
 
//Define Constant
 
//在24us定时中断中调用433M接收解码函数
 
 
 
//同步码，10ms低电平
 
#define C_RF_START_L_MAX        500        //12MS //10MS--10000/24=416
 
#define C_RF_START_L_MIN     250        //6MS
 
 
 
//之后是25位数据，只取24位3个字节
 
#define C_RF_REC_BIT_LEN    24    
 
//"1"是1.1ms高+0.2ms低, "0"是0.45高+0.9低
 
//所以可以只通过高电平判断
 
#define C_RF_DATA1_MAX        54        //1.3ms
 
#define C_RF_DATA1_MIN        37        //0.9ms
 
#define C_RF_DATA0_MAX        25        //0.6ms
 
#define C_RF_DATA0_MIN        13        //0.3ms
 
 
 
//结束码，短按：130ms低电平，长按：500ms，这里取大于70ms
 
//#define C_RF_END_L_MAX        500        //140MS
 
#define C_RF_END_L_MIN     2916        //70MS
 

 
#define C_RF_DATA_1_KEY     0x01        //A键值
 
#define C_RF_DATA_2_KEY     0x02        //B键值
 
#define C_RF_DATA_3_KEY     0x04        //C键值
 
#define C_RF_DATA_4_KEY     0x08        //D键值
 
 
 
#define C_DELAY_KEY_PRESS_SHORT        500        //1000*1MS
 
#define C_DELAY_KEY_PRESS_LONG        2000    //3000*1MS
 



extern void RF433M_Init(void);
extern void RF433M_RecevieDecode(void);
extern void Mod_Syn480r_Init(void);

#endif