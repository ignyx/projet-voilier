#ifndef __piloteGirouette_h
#define __piloteGirouette_h

#include "stm32f10x.h"

// Uses the timer and its TIMx_CH1 and TIMx_CH2 pins
// Enables the timer.
void Girouette_Init ( TIM_TypeDef * Timer ) ;

// returns alpha angle in degrees. Value will be between 0 and 180.
// MUST be called after Girouette_Init
int Girouette_GetAlpha();

// Configure Z channel interrupt
// Uses PA0 pin and EXTI0 interrupt line
void Girouette_InitCalibration_PA0 ();
#endif