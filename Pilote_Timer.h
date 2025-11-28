#ifndef __piloteTimer_h
#define __piloteTimer_h

#include "stm32f10x.h"

void MyTimer_Base_Init (TIM_TypeDef * Timer, unsigned short ValARR, unsigned short ValPSC);

#define MyTimer_Base_Start( Timer ) ((Timer)->CR1 |= TIM_CR1_CEN)
#define MyTimer_Base_Stop( Timer ) ((Timer)->CR1 &= ~TIM_CR1_CEN)

/*
**************************************************************************************************
* @brief
* @param : - TIM_TypeDef * Timer : Timer concerne
- char Prio : de 0 a 15
* @Note : La fonction MyTimer_Base_Init doit avoir ete lancee au prealable
**************************************************************************************************
*/

void MyTimer_ActiveIT(TIM_TypeDef * Timer, char Prio) ;

void MyTimer_PWM(TIM_TypeDef * Timer, char Channel);
void MyTimer_PWM_rate(TIM_TypeDef * Timer, char Channel, char rate);

#endif