#include "stm32f10x.h"
#include "Pilote_GPIO.h"
#include "Pilote_Timer.h"
#include "Pilote_ADC.h"
#include "Pilote_Girouette.h"
#include "Pilote_Ecoute.h"
#include "Pilote_Roulis.h"
#include "Pilote_Telecommande.h"
#include "Pilote_UART.h"

/////////////////////////////////
//  fonction 'pas top top'     //
//  d'attente d'environ 50 ms  //
/////////////////////////////////
void Attente (void)
{
	for (int i = 0 ; i < 200000 ; i++){}
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
	
	// UART3 : PB10 and PB11 
	// Send messages to the remote control : TIM2
	// Motor direction : PB4
	// Motor PWM : PA8
	// PWM : TIM1
	// ACD1 : PA1
	Start_Telecommande();

	while (1) {
		Attente();
		if (Roulis_isBoatInBadShape()) {
			Ecoute_setTheta(0);
			EnvoyerUART3("Attention chavirement !!!\n");
		} else
			Ecoute_setTheta(AlphaToTheta(Girouette_GetAlpha()));
	};
}