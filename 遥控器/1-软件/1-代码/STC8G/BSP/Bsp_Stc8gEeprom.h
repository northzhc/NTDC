#ifndef __BSP_STC8EEPROM_H
#define __BSP_STC8EEPROM_H

#include "STC8Gxx.h"
#include "Pub_TypeDefine.h"



extern void Bsp_Stc8gEeprom_Init(void);
extern void IapEeprom_Idle(void);
extern INT8U IapEeprom_Read(INT16U _uin_Addr);
extern void IapEeprom_Program(INT16U _uin_Addr, INT8U _uch_Data);
extern void IapEeprom_Erase(INT16U _uin_Addr);
extern void Eeprom_Test(void);
extern BOOL SavePara_Eeprom(INT8U _uch_Type);
extern void ReadPara_Eeprom(void);
#endif
