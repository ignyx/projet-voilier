#ifndef __piloteEcoute_h
#define __piloteEcoute_h

#include "stm32f10x.h"

// Setup and start PWM on Timer.
// Initial Angle: 0 degrees.
// Enables the timer
void Ecoute_init ( TIM_TypeDef * Timer ) ;

// Set theta angle.
// theta must be between 0 and 90 degrees inclusive
void Ecoute_setTheta(int theta);

// Calculte the expected theta depending on the alpha
int AlphaToTheta(int alpha);

#endif