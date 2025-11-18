#include "stm32f10x.h"
#include "Pilote_Timer.h"
#include "Pilote_GPIO.h"

static TIM_TypeDef * Girouette_Timer;

void Girouette_Init ( TIM_TypeDef * Timer ) {

	// We count rising edges on both A and B channels, so a full revolution is 2 * 360 degrees.
	
	MyTimer_Base_Init(Timer, 360 * 2, 1);
	// Set SMS to 3: Encoder triggered on changes from both channels TI1 and TI2
  Timer->SMCR &= ~TIM_SMCR_SMS;
	Timer->SMCR |= TIM_SMCR_SMS_0;
	Timer->SMCR |= TIM_SMCR_SMS_1;
	
	// Map TIMx_CH1 (pin PA7/D12 for TIM3) to TI1
	Timer->CCMR1 &=~TIM_CCMR1_CC1S ;
	Timer->CCMR1 |=TIM_CCMR1_CC1S_0 ;
	
	// Set sampling filter (from prof)
	Timer->CCMR1 &=~TIM_CCMR1_IC1F;
	Timer->CCMR1 |=TIM_CCMR1_IC1F_0;
	Timer->CCMR1 |=TIM_CCMR1_IC1F_3;
	
	// Map TIMx_CH2 (pin PA6/D11 for TIM3) to TI2
	Timer->CCMR1 &=~TIM_CCMR1_CC2S;
	Timer->CCMR1 |=TIM_CCMR1_CC2S_1;
	
	// Set sampling filter (from prof)
	Timer->CCMR1 &=~TIM_CCMR1_IC2F;
	Timer->CCMR1 |=TIM_CCMR1_IC2F_0;
	Timer->CCMR1 |=TIM_CCMR1_IC2F_3;
	
	Girouette_Timer = Timer;
	
	MyTimer_Base_Start(Timer);
}

void Girouette_InitCalibration_PA0 () {
	InitGPIO(GPIOA, 0, INPUT_PULLUP);
	
	// Add mask bit for EXTI0
	EXTI->IMR |= EXTI_IMR_MR0;
	
	// Listen for rising edge on EXTI0 (PA0)
	EXTI->RTSR |= EXTI_RTSR_TR0;
	
	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_SetPriority(EXTI0_IRQn, 5);
}

// This handler is called when a rising edge in on PA0, meaning the girouette is at 0 degrees.
void EXTI0_IRQHandler ( void ) {
	// Reset pending bit
	EXTI->PR |= EXTI_PR_PR0;
	
	// Set angle counter to 0
	Girouette_Timer->CNT = 0;
}


int Girouette_GetAlpha() {
	const int relative_degrees = Girouette_Timer->CNT / 2;

	if (relative_degrees <180){
		return relative_degrees;
	}	else {
		return 360 - relative_degrees;
	}
}

