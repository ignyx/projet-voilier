#ifndef __piloteTelecommande_c
#define __piloteTelecommande_c

#include "stm32f10x.h"
#include "Pilote_UART.h"
#include "Pilote_ADC.h"
#include "Pilote_Timer.h"
#include "Pilote_GPIO.h"
#include <stdio.h>

/////////////////////////////////
//  fonction 'pas top top'     //
//  d'attente d'environ 1 s //
/////////////////////////////////
void Wait (void)
{
	for (int i = 0 ; i < 4000000 ; i++){}
}


void Start_Telecommande() {
	//UART3 pour comm télécommande
	InitUART3();
	
	//start ADC pour batterie
	ADC_init(1); //ADC1, channel 1
	ADC_start();
	
	//TIM2 : 3 sec
	MyTimer_Base_Init(TIM2, 10000, 21600);
	MyTimer_ActiveIT(TIM2,5);
	MyTimer_Base_Start(TIM2);
	
//	//Moteur plateau
	InitGPIO(GPIOB, 4, OUTPUT_PUSHPULL); //B4 : Plateau Dir
	InitGPIO(GPIOA, 8, OUTPUT_ALTERNATE_PUSHPULL); //A8 : Plateau PWM
	MyTimer_Base_Init(TIM1, 359, 9); // freq=20kHz
	MyTimer_PWM(TIM1, 1);
	MyTimer_PWM_rate(TIM1, 1, 0);
	MyTimer_Base_Start(TIM1);
}


void Test_UART(void)
{
	InitUART3();
	char text[] = "Hello";
	while (1) {
		EnvoyerUART3(text);
		Wait();
	}
}

//void TIM1_UP_IRQHandler(void) {
//	//enlever le flag
//	TIM1->SR &= ~TIM_SR_UIF;
//	EnvoyerUART3("RAS");
//}

void TIM2_IRQHandler() { //toutes les 3s
	//enlever le flag
	TIM2->SR &= ~TIM_SR_UIF;
	EnvoyerUART3("RAS");
}

void ADC1_2_IRQHandler() {
	//Lire la mesure de batterie
	int batterie = 13*ADC1->DR; //proportion de batterie : 0-> 0V, 2^12 -> 12V
	if (batterie < (1<<12)/2) {
		EnvoyerUART3("Batterie trop faible.");
	}
}

void USART3_IRQHandler() {
	//Recevoir le degré de rotation
	signed char cap = USART3->DR;
	char cap_abs;
	if (cap<0) {
		SetBroche(GPIOB, 4);
		cap_abs = (char) -cap;
	} else {
		ResetBroche(GPIOB, 4);
		cap_abs = (char) cap;
	}
	MyTimer_PWM_rate(TIM1, 1, cap_abs);
	
//	Wait();
//	char string[20];
//	sprintf(string, "%d", cap);
//	EnvoyerUART3(string);
}



#endif