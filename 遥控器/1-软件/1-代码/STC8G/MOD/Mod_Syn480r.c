#include "Mod_Syn480r.h"
#include "Bsp_Stc8gEeprom.h"
#include "App_GlobalValue.h"
#include "Bsp_Stc8gUart.h"

/*
���ǿ����趨һ���Ͽ�Ķ�ʱ�жϣ������趨24us����
Ȼ�����е��ý��ս��뺯����ÿ���ж�ֻ�ж���Ӧ�ı�־��
��������ж�ִ��ʱ����������⡣

��������һ���շ�ģ�飬ͬ����Ϊ10ms�͵�ƽ��֮����25λ���ݣ�
ֻȡ24λ3���ֽڣ�"1"��1.1ms��+0.2ms��, "0"��0.45��+0.9�ͣ�
���Կ���ֻͨ���ߵ�ƽ�жϣ������Ǿ���ĳ�����Ҫע����Ǹ��ص������в���ִ��̫�ã�
��������ñ�־�ͺ��ˣ�
*/

INT8U    uch_RfRxStep;                //IR���ղ���
INT16U   uin_RfRxCnt;                //���ռ�����
 
INT32U   gul_RfRxData;            //���������ݴ棬//ͬ����֮����25λ���ݣ�ֻȡ24λ3���ֽ�

//INT8U    uch_RfRxData1;                //��������--//�ͻ�����1
//INT8U    uch_RfRxData2;                //�ͻ�����2 
//INT8U    uch_RfRxData3;                //������

 
 
INT8U    uch_RfRxLevel;
INT8U    uch_RfRxBitCnt;                //��������λ
BOOL     b_RfRecOkFlag;            //�յ�������24λ������1���൱��g_bitKeyDownFlag
//BOOL     b_RfRecEndCodeFlag;        //�յ�������
INT8U    uch_TimerCount;
INT8U    uch_RfRxKeeppingMsCnt;        //���յ�һ�����ݺ�ʼ�ļ�����
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
	RF_PIN = 1;//��1�ٶ�
	
}
/**
  *************************************************************************
  * @brief: RF433M_Init
  * @param: void
  * @return: void
  * @function:���ճ�ʼ������
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
  * @note:��һ���յ�������24λ���ݵĻص�����
		  �൱�ڰ������µ��ú��� -- OnkeyDown()
		  ֻ�ڿ�ʼ��ʱ���һ��
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
  * @note:�յ�������24λ���ݺ�Ļص����� 
		  �൱�ڰ������µ��ú��� -- OnkeyPressing()
		  �ڰ����ڼ䣬�᲻�Ͻ���
  *************************************************************************
 **/ 
void RF433M_OnRecevieData(void)
{
    if(uch_RfRxKeeppingMsCnt >= C_DELAY_KEY_PRESS_LONG)
    {
        //���ܴ���
    	if(gst_RfRecData.ul_RemoteCtlAddr == 0)
    	{
    		return;
    	}
		//ֻҪ�����е�ʱ��ſ����л�������״̬
	    if(gst_SystemPara.uch_SystemStatus == IDLE)
		{
	    	gst_SystemPara.uch_SystemStatus = RECV;//�л������ܽ���
	    	gst_SystemPara.uch_RemoteCtlPressSta = LONG_PRESS;//����
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
  * @note:	�յ�24λ���ݺ��ٽ��յ�������Ļص�����
			�൱�ڰ��������ɿ�ʱ���ú��� -- OnkeyUp()
  *************************************************************************
 **/ 
void RF433M_OnRecevieEndCode(void) 
{
    if(uch_RfRxKeeppingMsCnt <= C_DELAY_KEY_PRESS_SHORT) 
    {
    	//���մ���
    	if(gst_RfRecData.ul_RemoteCtlAddr == 0)
    	{
    		return;
    	}
   		 //ֻҪ�����е�ʱ��ſ����л�������״̬
    	if(gst_SystemPara.uch_SystemStatus == IDLE)
    	{
	    	gst_SystemPara.uch_SystemStatus = RECV;//�л������ܽ���
	    	gst_SystemPara.uch_RemoteCtlPressSta = SHORT_PRESS;//�̰�
    	}
		//uch_RfRxKeeppingMsCnt = 0;	
    }
}
 
/**
*************************************************************************
* @brief: RF433M_RecevieDecode
* @param: void
* @return: void
* @function:433���ս��뺯��
* @author: 
* @version: V1.0
* @note:��24us��ʱ�ж��е���433M���ս��뺯��
		ͬ���룬10ms�͵�ƽ
		֮����25λ���ݣ�ֻȡ24λ3���ֽ�
		"1"��1.1ms��+0.2ms��, "0"��0.45��+0.9��
		���Կ���ֻͨ���ߵ�ƽ�ж�
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
	if(uin_RfRxCnt > C_RF_START_L_MIN)//�ж��Ӳ�
	{
		uch_RfRxBitCnt = 0;
		gul_RfRxData = 0;
		return;
	}*/
    switch(uch_RfRxStep)
    {
        case    0:
            if(!RF_PIN)//�͵�ƽ
            {
                //1-1.��ʼ���������10ms�͵�ƽ�������100ms�͵�ƽ
                uin_RfRxCnt = 0;
                uch_RfRxStep = 1;
            }     
            break;
        case    1: 
            if(!RF_PIN) 
            { 
                //1-2.������10ms�������100ms�͵�ƽ��ʱ
                uin_RfRxCnt++; 
            }
            else
            {     
                //1-3.�ж�������9ms�͵�ƽ�������100ms�͵�ƽ 
                if((uin_RfRxCnt > C_RF_END_L_MIN) && b_RfRecOkFlag) 
                { 
                    //������ 
                    b_RfRecOkFlag = FALSE; 
                    //�൱�ڰ��������ɿ�ʱ���ú��� -- OnkeyUp() 
                    RF433M_OnRecevieEndCode(); 
                    return ; 
                } 
                else if((uin_RfRxCnt > C_RF_START_L_MAX) || (uin_RfRxCnt < C_RF_START_L_MIN)) 
                { 
                    goto F_RfRxError;             
                }
                //������             
                //b_RfRecOkFlag = 0;
                //2-1.��ʼ�������ݣ����������4.5ms�ߵ�ƽ     
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
                //��ƽ���ֲ��� 
                uin_RfRxCnt++; 
            } 
            else 
            {    //level change,check current level
                uch_RfRxLevel = RF_PIN; 
                if(!RF_PIN) 
                {     
                    //����0��1��ͨ���ߵ�ƽʱ���жϣ����Ե�ƽ����Ϊ�͵�ʱ�� 
                    //Ҳ�ͻ�ȡ���ߵ�ƽ��ʱ���ˣ� 
                    //�Ϳ����ж���Ч����0,1
                    gul_RfRxData = gul_RfRxData << 1; 
                    //"1"��1.1ms��+0.2ms��, "0"��0.45��+0.9�� 
                    //���Կ���ֻͨ���ߵ�ƽ�ж�,��Ȼ�õ͵�ƽ�ж�Ҳһ������ 
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
						//�������,�����յ����ݱ�־ 
//                        uch_RfRxData3 = gew_RfRxData $ 0;    //������
//                        uch_RfRxData2 = gew_RfRxData $ 1;    //�ͻ�����
//                        uch_RfRxData1 = gew_RfRxData $ 2;    //�ͻ�����

						gst_RfRecData.ul_RemoteCtlAddr = gul_RfRxData >> 4;//ң������ַ
						gst_RfRecData.uch_KeyNum = gul_RfRxData & 0x000F;//ң��������ֵ

						
                        //b_RfRecEndCodeFlag = FALSE; 
                        uch_RfRxStep = 0; 
                        uch_RfRxBitCnt=0; 
                        if(!b_RfRecOkFlag) 
                        { 
                            b_RfRecOkFlag = TRUE;; 
                            //��ʱ���൱�ڰ�����OnkeyDown() 
                            RF433M_OnRecevieFirstData(); 
                        } 
                        //��ʱ���൱�ڰ�����OnkeyPressing() 
                        RF433M_OnRecevieData(); 
                    } 
                } 
                uin_RfRxCnt = 0; 
            } 
            break; 
    }
}


