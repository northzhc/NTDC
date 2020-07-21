#ifndef __MOD_BUTTON_H
#define __MOD_BUTTON_H

#include "STC8Gxx.h"
#include "Pub_TypeDefine.h"
#define BUTTON_PIN P32//P55 //°´¼ü

#define BUTTON_SHORT  0x40
#define BUTTON_LONG   0x80

typedef enum
{
	NO_BUTTON_PRESS,
	BUTTON_PRESS,
}BUTTON_PRESS_STA_E;

extern void Mod_Button_Init(void);
extern void Button_Process(void);
#endif
