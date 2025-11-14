#ifndef __piloteADC_h
#define __piloteADC_h

#include "stm32f10x.h"

/*
* This implementation only supports ADC1.
*/



/*
* @brief Start ADC1. Configure first with ADC_init.
*/
void ADC_start();

/*
* @brief Turns off ADC1.
*/
void ADC_stop();

/*
* @brief Turn ADC1 on and configure continuous conversion on the specified channel.
* @param channel : ADC channel number (1-17). Undefined behavior for other value.
*/
void ADC_init(unsigned short channel);


#endif