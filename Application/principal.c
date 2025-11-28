#include "stm32f10x.h"
#include "Pilote_GPIO.h"
#include "Pilote_Timer.h"
#include "Pilote_ADC.h"
#include "Pilote_Girouette.h"
#include "Pilote_Ecoute.h"
#include "Pilote_Roulis.h"

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

	
	// SPI1 and TIM3 share same pins so we remap TIM3.
	// CH1: PC6 (opposite of D15)
	// CH2: PC7 (D9)
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	AFIO->MAPR |= AFIO_MAPR_TIM3_REMAP;
	Girouette_Init(TIM3);
	Girouette_InitCalibration_PA0();

	// TIM4 in on PB6
	Ecoute_init(TIM4);
	

	// SPI2 can't be used according to MySPI lib.
	// NSS:  PA4 (A2)
	// SCK:  PA5 (D13)
	// MISO: PA6 (D12)
	// MOSI: PA7 (D11)
	RCC->APB1ENR |= RCC_APB2ENR_SPI1EN;
	Roulis_init(SPI1);

	// Enable clock for timer TIM2, 72 MHz
	// 500ms is 36 000 000 clock cycles.
	MyTimer_Base_Init(TIM2, 360, 10000);
	MyTimer_Base_Start(TIM2);
	
	// Configure interrupt
	MyTimer_ActiveIT(TIM2, 2);
	while (1) {};
}

// called on TIM2 interruptions
void TIM2_IRQHandler ( void )
{
	// Reset update activation bit, should have been set by peripheral on event
	TIM2->SR &= ~TIM_SR_UIF;

	
	if (Roulis_isBoatInBadShape()) 
		Ecoute_setTheta(0);
	else
		Ecoute_setTheta(AlphaToTheta(Girouette_GetAlpha()));
}