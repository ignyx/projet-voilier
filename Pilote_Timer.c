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
		irq_number = TIM2_IRQn;
	

	// Allow TIMx events in interrupt controller and set priority
	NVIC_EnableIRQ(irq_number);
	NVIC_SetPriority(irq_number, Prio);

	// Enable Timer update events
	Timer->DIER |= TIM_DIER_UIE;
}