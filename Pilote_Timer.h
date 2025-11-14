#ifndef __piloteTimer_h
#define __piloteTimer_h

#include "stm32f10x.h"

void MyTimer_Base_Init ( TIM_TypeDef * Timer , unsigned short ValARR , unsigned short ValPSC ) ;

#define MyTimer_Base_Start( Timer ) ((Timer)->CR1 |= TIM_CR1_CEN)
#define MyTimer_Base_Stop( Timer ) ((Timer)->CR1 &= ~TIM_CR1_CEN)

/*
**************************************************************************************************
* @brief
* @param : - TIM_TypeDef * Timer : Timer concerne
- char P r i o : de 0 a 15
* @Note : La f o n c t i o n MyTimer_Base_Init d o i t a v o i r e t e l a n c e e au p r e a l a b l e
**************************************************************************************************
*/
void MyTimer_ActiveIT ( TIM_TypeDef * Timer , char Prio ) ;

#endif