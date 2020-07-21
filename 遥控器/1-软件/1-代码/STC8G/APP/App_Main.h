#ifndef __APP_MAIN_H
#define __APP_MAIN_H

#include "STC8Gxx.h"
/*APP*/
#include "App_GlobalValue.h"
/*BSP*/
#include "Bsp_Stc8gGpio.h"
#include "Bsp_Stc8gTimer.h"
#include "Bsp_Stc8gUart.h"
#include "Bsp_Stc8gClock.h"
#include "Bsp_Stc8gEeprom.h"
/*MOD*/
#include "Mod_Syn480r.h"
#include "Mod_Button.h"
/*PUB*/
#include "Pub_Delay.h"

extern void App_Process(void);
extern void RemoteCtl_Process(void);
extern void RemoteCtlOutput_Config(void);

#endif

