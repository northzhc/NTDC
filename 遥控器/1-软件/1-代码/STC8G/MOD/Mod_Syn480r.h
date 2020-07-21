#ifndef __MOD_SYN480R_H
#define __MOD_SYN480R_H

#include "STC8Gxx.h"
#include "Pub_TypeDefine.h"

/***************************************************/
 
//Define I/O Register
#define RF_PIN  P54
 
//Define Constant
 
//��24us��ʱ�ж��е���433M���ս��뺯��
 
 
 
//ͬ���룬10ms�͵�ƽ
 
#define C_RF_START_L_MAX        500        //12MS //10MS--10000/24=416
 
#define C_RF_START_L_MIN     250        //6MS
 
 
 
//֮����25λ���ݣ�ֻȡ24λ3���ֽ�
 
#define C_RF_REC_BIT_LEN    24    
 
//"1"��1.1ms��+0.2ms��, "0"��0.45��+0.9��
 
//���Կ���ֻͨ���ߵ�ƽ�ж�
 
#define C_RF_DATA1_MAX        54        //1.3ms
 
#define C_RF_DATA1_MIN        37        //0.9ms
 
#define C_RF_DATA0_MAX        25        //0.6ms
 
#define C_RF_DATA0_MIN        13        //0.3ms
 
 
 
//�����룬�̰���130ms�͵�ƽ��������500ms������ȡ����70ms
 
//#define C_RF_END_L_MAX        500        //140MS
 
#define C_RF_END_L_MIN     2916        //70MS
 

 
#define C_RF_DATA_1_KEY     0x01        //A��ֵ
 
#define C_RF_DATA_2_KEY     0x02        //B��ֵ
 
#define C_RF_DATA_3_KEY     0x04        //C��ֵ
 
#define C_RF_DATA_4_KEY     0x08        //D��ֵ
 
 
 
#define C_DELAY_KEY_PRESS_SHORT        500        //1000*1MS
 
#define C_DELAY_KEY_PRESS_LONG        2000    //3000*1MS
 



extern void RF433M_Init(void);
extern void RF433M_RecevieDecode(void);
extern void Mod_Syn480r_Init(void);

#endif