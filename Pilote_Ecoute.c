#include "stm32f10x.h"
#include "Pilote_Timer.h"
#include "Pilote_GPIO.h"

static TIM_TypeDef * PWM_Timer;

// Set theta angle.
// theta must be between 0 and 90 degrees inclusive
void Ecoute_setTheta(int theta) {
	// ARR is 1800 so the first millisecond is 90
	PWM_Timer->CCR1 = 90 + theta;
	// trigger an update event so shadow register gets updated
	PWM_Timer->EGR |= TIM_EGR_UG;
}

// Setup and start PWM on Timer.
// Initial Angle: 0 degrees.
// Enables the timer
void Ecoute_init ( TIM_TypeDef * Timer ) {
	PWM_Timer = Timer;
	// Clock is 72 MHz.
	// PSC is 800 so 9000 increments per second.
	// ARR is 1800.
	// 20 ms is 1800.
	// 2 ms is 180.
	MyTimer_Base_Init(Timer, 1800, 800); // TODO set PSC, ARR
	// In PWM mode 1, when the counter reaches ARR (goes to 0),
	// the PWM signal is set (high). When the counter reaches CCR,
	// the PWM signal is reset (low).
	
	
	Ecoute_setTheta(0);
	
	// Config CC1 channel as output (00)
	Timer->CCMR1 &= ~TIM_CCMR1_CC1S;
	// Enable preload feature so the CCR1 value is reloaded on update event
	Timer->CCMR1 |= TIM_CCMR1_OC1PE;
	// Config as PWM in mode 1 (110)
	Timer->CCMR1 |= TIM_CCMR1_OC1M;
	Timer->CCMR1 &= ~TIM_CCMR1_OC1M_0;
	
	
	// Enable Capture/Compare OC1 so signal is output to pin
	Timer->CCER |= TIM_CCER_CC1E;
	
	InitGPIO(GPIOB, 6, OUTPUT_ALTERNATE_PUSHPULL);
	
	// TIM4_CH1 is on pin PB6
	// TODO setup output pin to alternate 
	// See PWM for concept
	// See dossier pour les valeur pour le servomoteur.
	// Ce que l'on envoie est la commande (l'angle désiré), pas la tension à envoyer au moteur.
	
	// send UG bit to copy register values to shadow registers
	Timer->EGR |= TIM_EGR_UG;
	MyTimer_Base_Start(Timer);
}



//Take the alpha and give the conresponding theta
int AlphaToTheta(int alpha){
	if (alpha>=45) {
		return (alpha-45)*90/135;//135=180-45
	}
	else {//we assume the angle is between 0 and 45
		return 0;
	}
}
