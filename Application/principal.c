#include "stm32f10x.h"
#include "Pilote_GPIO.h"
#include "Pilote_Timer.h"
#include "Pilote_ADC.h"

/////////////////////////////////
//  fonction 'pas top top'     //
//  d'attente d'environ 250 ms //
/////////////////////////////////
void Attente (void)
{
	for (int i = 0 ; i < 1000000 ; i++){}
}
/////////////////////////////////
int main (void) {
	// The goal of this program is to flash an onboard Nucleo LED every 500ms
	
	// Onboard LED is on GPIO PA5
	InitGPIO(GPIOA, 5, OUTPUT_PUSHPULL);
	InitGPIO(GPIOA, 1, INPUT_ANALOG);
	
	
	// Enable clock for timer TIM2, 72 MHz
	// 500ms is 36 000 000 clock cycles.
	MyTimer_Base_Init(TIM2, 3600, 10000);
	MyTimer_Base_Start(TIM2);
	
	// Configure interrupt
	MyTimer_ActiveIT(TIM2, 2);
	
	
	// To test the code below:
	// - Add the card on top of the nucleo card
	// - plug the potentiometer onto the A1
	// - in the debugger, open peripherals > ADC1
	// - See the changes in the DR register
	
	// GPIO A1 maps to ADC channel IN1 (see Datasheet (NOT the ref),  pin definitions)
	ADC_init(1);
	ADC_start();
	
	while (1) {};
}

/* Old main bfore using ADC driver
int main (void) {
	// The goal of this program is to flash an onboard Nucleo LED every 500ms
	
	// Onboard LED is on GPIO PA5
	InitGPIO(GPIOA, 5, OUTPUT_PUSHPULL);
	InitGPIO(GPIOA, 1, INPUT_ANALOG);
	
	
	// Enable clock for timer TIM2, 72 MHz
	// 500ms is 36 000 000 clock cycles.
	MyTimer_Base_Init(TIM2, 3600, 10000);
	MyTimer_Base_Start(TIM2);
	
	// Configure interrupt
	MyTimer_ActiveIT(TIM2, 2);
	
	
	// To test the code below:
	// - Add the card on top of the nucleo card
	// - plug the potentiometer onto the A1
	// - in the debugger, open peripherals > ADC1
	// - See the changes in the DR register
	
	// GPIO A1 maps to ADC channel IN1 (see Datasheet (NOT the ref),  pin definitions)

	// Enable ADC:
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
	ADC1->SQR3 |= ADC_SQR3_SQ1_0;
	// Set continuous conversion mode
	ADC1->CR2 |= ADC_CR2_CONT;
	// La partie échantillonneur bloqueur est paramétrée par le registre ADC_SMPR1 et ADC_SMPR2.
	// Set sample time to 28.5 cycles
	ADC1->SMPR2 &= ~ADC_SMPR2_SMP0;
	ADC1->SMPR2 |= ADC_SMPR2_SMP0_0;
	ADC1->SMPR2 |= ADC_SMPR2_SMP0_1;
	
	
	// Turn on temp sensor
	//ADC1->CR2 |= ADC_CR2_TSVREFE;
	// Start conversion
	ADC1->CR2 |= ADC_CR2_ADON;
	
	while (1) {};
}*/

/* Old main before using Timer driver
int main (void) {
	// The goal of this program is to flash an onboard Nucleo LED every 500ms
	
	// Onboard LED is on GPIO PA5
	InitGPIO(GPIOA, 5, OUTPUT_PUSHPULL);
	
	
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
	
	
	while (1) {};
}

*/
int ld2_active = 0;

// called on TIM2 interruptions
void TIM2_IRQHandler ( void )
{
	// Reset update activation bit, should have been set by peripheral on event
	TIM2->SR &= ~TIM_SR_UIF;
	
	// Toggle LED
	if (ld2_active == 1) {
		ResetBroche(GPIOA, 5);
		ld2_active = 0;
	} else {
		SetBroche(GPIOA, 5);
		ld2_active = 1;
	}
}

// Nouvelle version avec input pull-up/pull-down, ouput open-drain
int main_gpio_activite1 ( void )
{
	/* Old setup, now using driver
	//RCC->APB2ENR |= (1<<4) | RCC_APB2ENR_IOPAEN ;
	// Start clocks for GPIO A B and C
	RCC->APB2ENR |= (0x01 << 2) |(0x01 << 3) |(0x01 << 4);
	// Set PC8 (button) as Input pull-up (10)
	GPIOC->CRH &= ~GPIO_CRH_MODE8; //set bit 1 and 0 to 00 (Input)
	GPIOC->CRH &= ~GPIO_CRH_CNF8_0; // set bit 2 to 0. ~ is the bitwise not operator
	GPIOC->CRH |= GPIO_CRH_CNF8_1; // set bit 3 to 1
	GPIOC->ODR |= GPIO_ODR_ODR8; // setting outpit bit configures as pull-up
	// Set PA6 (led) as Output push-pull (00)
	GPIOA->CRL &= ~GPIO_CRL_MODE6;//set bit 24 and 25 to 00 (Output 2 MHz)
	GPIOA->CRL |= GPIO_CRL_MODE6_1; // set bit 25 to 1
	GPIOA->CRL &= ~GPIO_CRL_CNF6; // set 26 and 27 to 00
	GPIOA->CRL |= GPIO_CRL_CNF6_0; // set bit 26 to 1
	*/
	InitGPIO(GPIOC, 8, INPUT_PULLUP);
	InitGPIO(GPIOA, 6, OUTPUT_PUSHPULL);
	while (1) {
		if (LireBroche(GPIOC, 8))
			SetBroche(GPIOA, 6);
		else
			ResetBroche(GPIOA, 6);
		
		/*
		// Reset LED output bit to 0;
		GPIOA->ODR &= ~GPIO_ODR_ODR6;
		// Copy the Button input bit to the LED output bit
		// We shift bits from input bit 8 to output bit 6
		GPIOA->ODR |= (GPIO_IDR_IDR8 & GPIOC->IDR) >> 2;
			*/
		
		Attente();
	}
}



// Ancienne version avec inputpull-up/pull-down et output push-pull
/*
int main ( void )
{
	//RCC->APB2ENR |= (1<<4) | RCC_APB2ENR_IOPAEN ;
	// Start clocks for GPIO A B and C
	RCC->APB2ENR |= (0x01 << 2) |(0x01 << 3) |(0x01 << 4);
	// Set PC8 (button) as Input pull-up/pull-down (10)
	GPIOC->CRH &= ~GPIO_CRH_MODE8; //set bit 1 and 0 to 00 (Input)
	GPIOC->CRH &= ~GPIO_CRH_CNF8_0; // set bit 2 to 0. ~ is the bitwise not operator
	GPIOC->CRH |= GPIO_CRH_CNF8_1; // set bit 3 to 1
	// Set PA6 (led) as Output push-pull (00)
	GPIOA->CRL &= ~GPIO_CRL_MODE6;//set bit 24 and 25 to 00 (Output 2 MHz)
	GPIOA->CRL |= GPIO_CRL_MODE6_1; // set bit 25 to 1
	GPIOA->CRL &= ~GPIO_CRL_CNF6; // set 26 and 27 to 00
	while (1) {		
		// Reset LED output bit to 0;
		GPIOA->ODR &= ~GPIO_ODR_ODR6;
		// Copy the Button input bit to the LED output bit
		// We shift bits from input bit 8 to output bit 6
		GPIOA->ODR |= (GPIO_IDR_IDR8 & GPIOC->IDR) >> 2;
			
		
		Attente();
	}
}
*/