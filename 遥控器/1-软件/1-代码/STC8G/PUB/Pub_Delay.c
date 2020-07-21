#include "Pub_Delay.h"
/**
 *************************************************************************
 * @brief: Delay_Ms
 * @param: _ul_Ms -- ∫¡√Î
 * @return: void
 * @function:
 * @author: 
 * @version: V1.0
 * @note:
 *************************************************************************
**/
void Delay_Ms(INT32U _ul_Ms)
{
    while(_ul_Ms--)
    {
		unsigned char i, j;

		i = 15;
		j = 90;
		do
		{
			while (--j);
		} while (--i);
    }
    
}
/**
 *************************************************************************
 * @brief: Delay_Us
 * @param: _ul_Us -- Œ¢√Î
 * @return: void
 * @function:
 * @author: 
 * @version: V1.0
 * @note:
 *************************************************************************
**/
//void Delay_Us(INT32U _ul_Us)
//{
//    while(_ul_Us--)
//    {
//        _nop_();
//    }
//}