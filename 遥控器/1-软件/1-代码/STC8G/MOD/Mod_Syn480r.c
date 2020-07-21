#include "Mod_Syn480r.h"
#include "Bsp_Stc8gEeprom.h"
#include "App_GlobalValue.h"
#include "Bsp_Stc8gUart.h"

/*
我们可以设定一个较快的定时中断（下面设定24us），
然后在中调用接收解码函数，每次中断只判断相应的标志，
不会造成中断执行时间过长的问题。

比如其中一对收发模块，同步码为10ms低电平，之后是25位数据，
只取24位3个字节，"1"是1.1ms高+0.2ms低, "0"是0.45高+0.9低，
所以可以只通过高电平判断，下面是具体的程序，需要注意的是各回调函数中不能执行太久，
最好是设置标志就好了：
*/

INT8U    uch_RfRxStep;                //IR接收步骤
INT16U   uin_RfRxCnt;                //接收计数器
 
INT32U   gul_RfRxData;            //接收数据暂存，//同步码之后是25位数据，只取24位3个字节

//INT8U    uch_RfRxData1;                //接收数据--//客户代码1
//INT8U    uch_RfRxData2;                //客户代码2 
//INT8U    uch_RfRxData3;                //数据码

 
 
INT8U    uch_RfRxLevel;
INT8U    uch_RfRxBitCnt;                //接收数据位
BOOL     b_RfRecOkFlag;            //收到完整的24位数据置1，相当于g_bitKeyDownFlag
//BOOL     b_RfRecEndCodeFlag;        //收到结束码
INT8U    uch_TimerCount;
INT8U    uch_RfRxKeeppingMsCnt;        //接收第一个数据后开始的计数器
//INT8U    uch_RfRxKeeppingIntervalCount;
// 
//BOOL     b_ResetFlag;

/**
 *************************************************************************
 * @brief: Mod_Syn480r_Init
 * @param: void
 * @return: void
 * @function:
 * @author: 
 * @version: V1.0
 * @note:
 *************************************************************************
**/
void Mod_Syn480r_Init(void)
{
	RF433M_Init();
	RF_PIN = 1;//置1再读
	
}
/**
  *************************************************************************
  * @brief: RF433M_Init
  * @param: void
  * @return: void
  * @function:接收初始化函数
  * @author: 
  * @version: V1.0
  * @note:
  *************************************************************************
 **/ 
void RF433M_Init(void)
{
    uch_RfRxStep = 0;
    b_RfRecOkFlag = FALSE;
    //b_RfRecEndCodeFlag = FALSE;
	uch_TimerCount = 0;
 
}    

/**
  *************************************************************************
  * @brief: RF433M_OnRecevieFirstData
  * @param: void
  * @return: void
  * @function:
  * @author: 
  * @version: V1.0
  * @note:第一次收到完整的24位数据的回调函数
		  相当于按键按下调用函数 -- OnkeyDown()
		  只在开始的时候进一次
  *************************************************************************
 **/ 
void RF433M_OnRecevieFirstData(void)
{
    uch_RfRxKeeppingMsCnt = 0; 
}
/**
  *************************************************************************
  * @brief: RF433M_OnRecevieData
  * @param: void
  * @return: void
  * @function:
  * @author: 
  * @version: V1.0
  * @note:收到完整的24位数据后的回调函数 
		  相当于按键按下调用函数 -- OnkeyPressing()
		  在按下期间，会不断进入
  *************************************************************************
 **/ 
void RF433M_OnRecevieData(void)
{
    if(uch_RfRxKeeppingMsCnt >= C_DELAY_KEY_PRESS_LONG)
    {
        //接受错误
    	if(gst_RfRecData.ul_RemoteCtlAddr == 0)
    	{
    		return;
    	}
		//只要当空闲的时候才可以切换到接受状态
	    if(gst_SystemPara.uch_SystemStatus == IDLE)
		{
	    	gst_SystemPara.uch_SystemStatus = RECV;//切换到接受解析
	    	gst_SystemPara.uch_RemoteCtlPressSta = LONG_PRESS;//长按
        }
    	//uch_RfRxKeeppingMsCnt = 0;
    }
}

/**
  *************************************************************************
  * @brief: RF433M_OnRecevieEndCode
  * @param: void
  * @return: void
  * @function:
  * @author: 
  * @version: V1.0
  * @note:	收到24位数据后，再接收到结束码的回调函数
			相当于按键按下松开时调用函数 -- OnkeyUp()
  *************************************************************************
 **/ 
void RF433M_OnRecevieEndCode(void) 
{
    if(uch_RfRxKeeppingMsCnt <= C_DELAY_KEY_PRESS_SHORT) 
    {
    	//接收错误
    	if(gst_RfRecData.ul_RemoteCtlAddr == 0)
    	{
    		return;
    	}
   		 //只要当空闲的时候才可以切换到接受状态
    	if(gst_SystemPara.uch_SystemStatus == IDLE)
    	{
	    	gst_SystemPara.uch_SystemStatus = RECV;//切换到接受解析
	    	gst_SystemPara.uch_RemoteCtlPressSta = SHORT_PRESS;//短按
    	}
		//uch_RfRxKeeppingMsCnt = 0;	
    }
}
 
/**
*************************************************************************
* @brief: RF433M_RecevieDecode
* @param: void
* @return: void
* @function:433接收解码函数
* @author: 
* @version: V1.0
* @note:在24us定时中断中调用433M接收解码函数
		同步码，10ms低电平
		之后是25位数据，只取24位3个字节
		"1"是1.1ms高+0.2ms低, "0"是0.45高+0.9低
		所以可以只通过高电平判断
*************************************************************************
**/
void RF433M_RecevieDecode(void)
{
	uch_TimerCount++;
    if(uch_TimerCount == 42) //1ms=42*24us
    {
        uch_TimerCount = 0;
        uch_RfRxKeeppingMsCnt++;
    } 
	/*
	if(uin_RfRxCnt > C_RF_START_L_MIN)//判断杂波
	{
		uch_RfRxBitCnt = 0;
		gul_RfRxData = 0;
		return;
	}*/
    switch(uch_RfRxStep)
    {
        case    0:
            if(!RF_PIN)//低电平
            {
                //1-1.开始检测引导码10ms低电平或结束码100ms低电平
                uin_RfRxCnt = 0;
                uch_RfRxStep = 1;
            }     
            break;
        case    1: 
            if(!RF_PIN) 
            { 
                //1-2.引导码10ms或结束码100ms低电平计时
                uin_RfRxCnt++; 
            }
            else
            {     
                //1-3.判断引导码9ms低电平或结束码100ms低电平 
                if((uin_RfRxCnt > C_RF_END_L_MIN) && b_RfRecOkFlag) 
                { 
                    //结束码 
                    b_RfRecOkFlag = FALSE; 
                    //相当于按键按下松开时调用函数 -- OnkeyUp() 
                    RF433M_OnRecevieEndCode(); 
                    return ; 
                } 
                else if((uin_RfRxCnt > C_RF_START_L_MAX) || (uin_RfRxCnt < C_RF_START_L_MIN)) 
                { 
                    goto F_RfRxError;             
                }
                //引导码             
                //b_RfRecOkFlag = 0;
                //2-1.开始接收数据，检测引导码4.5ms高电平     
                uch_RfRxLevel = RF_PIN; 
                uin_RfRxCnt = 0; 
                uch_RfRxBitCnt = 0; 
                uch_RfRxStep = 2; 
                //gul_RfRxData = 0;
            } 
            break;
        case    2://check level change 
            if(RF_PIN == uch_RfRxLevel) 
            { 
                //电平保持不变 
                uin_RfRxCnt++; 
            } 
            else 
            {    //level change,check current level
                uch_RfRxLevel = RF_PIN; 
                if(!RF_PIN) 
                {     
                    //数据0、1是通过高电平时间判断，所以电平跳变为低的时候， 
                    //也就获取到高电平的时间了， 
                    //就可以判断有效数据0,1
                    gul_RfRxData = gul_RfRxData << 1; 
                    //"1"是1.1ms高+0.2ms低, "0"是0.45高+0.9低 
                    //所以可以只通过高电平判断,当然用低电平判断也一样类似 
                    if((uin_RfRxCnt < C_RF_DATA1_MAX) && (uin_RfRxCnt > C_RF_DATA1_MIN)) 
                    {//data 1 
                        //set1 gul_RfRxData.0; 
                        gul_RfRxData |= 1 << 0;
                        goto F_RfRxCheckBit; 
                    }     
                    else if((uin_RfRxCnt < C_RF_DATA0_MAX) && (uin_RfRxCnt > C_RF_DATA0_MIN)) 
                    {//data 0 
                        //set0 gul_RfRxData.0; 
                        gul_RfRxData &= ~(1 << 0);
                        goto F_RfRxCheckBit;                     
                    } 
                    else 
                    { 
                        goto F_RfRxError; 
                    }
F_RfRxError: 
                    uch_RfRxStep = 0;                 
                    return;
F_RfRxCheckBit: 
                    uch_RfRxBitCnt++; 
                    if(uch_RfRxBitCnt >= C_RF_REC_BIT_LEN) 
                    {    
						//接收完成,设置收到数据标志 
//                        uch_RfRxData3 = gew_RfRxData $ 0;    //数据码
//                        uch_RfRxData2 = gew_RfRxData $ 1;    //客户代码
//                        uch_RfRxData1 = gew_RfRxData $ 2;    //客户代码

						gst_RfRecData.ul_RemoteCtlAddr = gul_RfRxData >> 4;//遥控器地址
						gst_RfRecData.uch_KeyNum = gul_RfRxData & 0x000F;//遥控器按键值

						
                        //b_RfRecEndCodeFlag = FALSE; 
                        uch_RfRxStep = 0; 
                        uch_RfRxBitCnt=0; 
                        if(!b_RfRecOkFlag) 
                        { 
                            b_RfRecOkFlag = TRUE;; 
                            //这时候相当于按键的OnkeyDown() 
                            RF433M_OnRecevieFirstData(); 
                        } 
                        //这时候相当于按键的OnkeyPressing() 
                        RF433M_OnRecevieData(); 
                    } 
                } 
                uin_RfRxCnt = 0; 
            } 
            break; 
    }
}


