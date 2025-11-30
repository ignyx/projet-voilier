#include "stm32f10x.h"
#include "Pilote_Timer.h"

void MyTimer_Base_Init ( TIM_TypeDef * Timer , unsigned short ValARR , unsigned short ValPSC ) {
	// Enable clock for timer TIMx, 72 MHz
	if (Timer == TIM2)
		RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	else if (Timer == TIM3)
		RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	else if (Timer == TIM4)
		RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	else if (Timer == TIM1) {
		RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
		RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
		RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
		TIM1->BDTR |= TIM_BDTR_MOE;
	}
	// TIM5 not available on this device ? not enabled in stm32f10x.h
	
	// Prescaler: counter is incremented every PSC clock cycles
	Timer->PSC = ValPSC;
	// Autoreload: when counter reaches ARR, counter resets and interrupt is triggered (if configured).
	Timer->ARR = ValARR;
}

	// See the short manual for info
void MyTimer_ActiveIT ( TIM_TypeDef * Timer , char Prio ) {
	IRQn_Type irq_number; // see NVIC vector table
	if (Timer == TIM2)
		irq_number = TIM2_IRQn; 
	else if (Timer == TIM3)
		irq_number = TIM3_IRQn;
	else if (Timer == TIM4)
		irq_number = TIM4_IRQn; 
	else
		irq_number = TIM1_UP_IRQn;
		TIM1->DIER |= TIM_DIER_CC1IE | TIM_DIER_BIE;
	

	// Allow TIMx events in interrupt controller and set priority
	NVIC_EnableIRQ(irq_number);
	NVIC_SetPriority(irq_number, Prio);

	// Enable Timer update events
	Timer->DIER |= TIM_DIER_UIE;
}

void MyTimer_PWM(TIM_TypeDef * Timer, char Channel){
	if (Channel == 1) {
		TIM1->CCMR1 &= ~TIM_CCMR1_OC1M;
		TIM1->CCMR1 |= TIM_CCMR1_OC1PE;
		Timer->CCMR1 &= 0b000 << 4;
		Timer->CCMR1 |= 0b110 << 4;
	} else if (Channel == 2) {
		Timer->CCMR1 &= 0b000 << 12;
		Timer->CCMR1 |= 0b110 << 12;
	} else if (Channel == 3) {
		Timer->CCMR2 &= 0b000 << 4;
		Timer->CCMR2 |= 0b110 << 4;
	} else {
		Timer->CCMR2 &= 0b000 << 12;
		Timer->CCMR2 |= 0b110 << 12;
	}	
	Timer->CCER |= 0b1 <<0 ;
}

void MyTimer_PWM_rate(TIM_TypeDef * Timer, char Channel, char rate) {
	unsigned short arr = Timer-> ARR;
	unsigned short ccr = arr * rate / 100 ;
	
	if (Channel == 1) {
		Timer->CCR1 = ccr;
	} else if (Channel == 2) {
		Timer->CCR2 = ccr;
	} else if (Channel == 3) {
		Timer->CCR3 = ccr;
	} else {
		Timer->CCR4 = ccr;
	}
}
