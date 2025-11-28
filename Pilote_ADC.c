#include "stm32f10x.h"
#include "Pilote_ADC.h"
#include "Pilote_GPIO.h"

void ADC_start(){
	// turn on ADC (section 11.3.1)
	ADC1->CR2 |= ADC_CR2_ADON;
}

void ADC_stop(){
	// turn off ADC
	(ADC1->CR2 &= ~ADC_CR2_ADON);
}

void ADC_init(unsigned short channel){
	// Enable ADC1 clock
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	// Set ADC1 clock prescaler to divide frequency by 8
	RCC->CFGR |= RCC_CFGR_ADCPRE_DIV8;
	// Turn on ADC (section 11.3.1)
	ADC1->CR2 |= ADC_CR2_ADON;
	// Set number of regular conversions (1). set 000.
	ADC1->SQR1 &= ~ADC_SQR1_L;
	// Select channel 1 (regular)
	ADC1->SQR3 &= ~ADC_SQR3_SQ1;
	ADC1->SQR3 |= channel; // TODO test
	InitGPIO(GPIOA, 1, INPUT_FLOATING);
	// Set continuous conversion mode
	ADC1->CR2 |= ADC_CR2_CONT;
	// La partie échantillonneur bloqueur est paramétrée par le registre ADC_SMPR1 et ADC_SMPR2.
	// Set sample time to 28.5 cycles
	ADC1->SMPR2 &= ~ADC_SMPR2_SMP0;
	ADC1->SMPR2 |= ADC_SMPR2_SMP0_0;
	ADC1->SMPR2 |= ADC_SMPR2_SMP0_1;
}
