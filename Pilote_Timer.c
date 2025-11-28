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
	
	/* Can be deleted, left if need to roll back
	// Because we can't easily infer the Timer number and corresponding clock bit, we enable all the general purpose timers.
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // Enable clock for timer TIM2, 72 MHz
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;	// Enable clock for timer TIM3, 72 MHz
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;	// Enable clock for timer TIM4, 72 MHz
	*/
	
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


/*
	if (Timer == TIM2) {
		NVIC->ISER[0] |= NVIC_ISER_SETENA_28;
		// Each IPR register contains 4 bytes.
		// 28 = 4*7 + 0 so TIM2 is the first byte of register 7
		NVIC->IPR[7] &= ~0xFF; // reset
		NVIC->IPR[7] |= Prio << 4; // set
	} else if (Timer == TIM3) {
		NVIC->ISER[0] |= NVIC_ISER_SETENA_29;
		// 29 = 4*7 + 1 so TIM2 is the second byte of register 7
		NVIC->IPR[7] &= ~(0xFF << 8); // reset
		NVIC->IPR[7] |= Prio << 8; // set
	} else if (Timer == TIM4) {
		NVIC->ISER[0] |= NVIC_ISER_SETENA_30;
		NVIC->IPR[7] &= ~(0xFF << 16);
		NVIC->IPR[7] |= Prio << 16;
  }
*/


/*
	// Enable clock for timer TIM2, 72 MHz ?
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	// Clock frequency is 72 MHz, so 36 000 000 clock cycles is 500 ms
	// We start with PSC = 10 000 (16 bit register)
	TIM2->PSC = 10000;
	// So we 
	TIM2->ARR = 3600;
	// Enable timer TIM2
	TIM2->CR1 |= TIM_CR1_CEN;
	
	// Configure interrupt
	// See the short manual for info
	
	// Enable TIM2 events in interrupt controller
	NVIC->ISER[0] |= NVIC_ISER_SETENA_28;
	// Enable Timer update events
	TIM2->DIER |= TIM_DIER_UIE;
	*/