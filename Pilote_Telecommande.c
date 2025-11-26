#ifndef __piloteTelecommande_c
#define __piloteTelecommande_c

#include "stm32f10x.h"
#include "Pilote_UART.h"
#include "Pilote_ADC.h"
#include "Pilote_Timer.h"

/////////////////////////////////
//  fonction 'pas top top'     //
//  d'attente d'environ 1 s //
/////////////////////////////////
void Attente (void)
{
	for (int i = 0 ; i < 4000000 ; i++){}
}


void start_telecommande() {
	//UART3 pour comm télécommande
	InitUART3();
	
	//start ADC pour batterie
	ADC_init(1); //ADC1, channel 1
	ADC_start();
	
	//TIM2 : 3 sec
	MyTimer_Base_Init(TIM2, 10000, 21600);
	MyTimer_ActiveIT(TIM2,5);
	MyTimer_Base_Start(TIM2);
}


void Test_UART(void)
{
	InitUART3();
	char text[] = "Hello";
	while (1) {
		EnvoyerUART3(text);
		Attente();
	}
}


void TIM2_IRQHandler() { //toutes les 3s
	//enlever le flag
	TIM2->SR &= ~TIM_SR_UIF;
	//Lire la mesure de batterie
	int batterie = 13*ADC1->DR; //proportion de batterie : 0-> 0V, 2^12 -> 12V
	if (batterie < (1<<12)/2) {
		EnvoyerUART3("Batterie trop faible.");
	} else {
		EnvoyerUART3("RAS");
	}
}


void USART3_IRQHandler() {
	//Recevoir le degré de rotation
	signed char cap = USART3->DR;
	/*
	char s[7];
	sprintf(s,"%c",cap);
	Attente();
	EnvoyerUART3(s);*/
	// To do : fonction pour actionner moteur et faire tourner plateau
}



#endif