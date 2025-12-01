#ifndef __piloteHorloge_h
#define __piloteHorloge_h

#include "stm32f10x.h"

// Setup I2C connection
void Horloge_init (I2C_TypeDef  * i2c ) ;

// Get datetime in human string format
// MUST be called after Horloge_init
void Horloge_GetTimeString(char result[10]) ;

#endif