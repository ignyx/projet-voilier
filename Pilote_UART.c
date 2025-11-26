#ifndef __piloteUART_c
#define __piloteUART_c

#include "stm32f10x.h"
#include "Pilote_GPIO.h"

void InitUART3() {
	// Start clocks for USART 2 et 3 and then USART1
	RCC->APB1ENR |= (0x01 << 18);
	//Configure les pins TX et RX
	InitGPIO(GPIOB, 10, OUTPUT_ALTERNATE_PUSHPULL);
	InitGPIO(GPIOB, 11, INPUT_FLOATING);
	//Set the Uart Enable (UE) bit
	USART3->CR1 |= (0x01 << 13);
	//Baud rate = 9600 / USARTDIV = 3750 / mantissa=0xEA & fraction=0x6
	USART3->BRR = 0xEA6;
	//Parameters : stop bits=1 ; word length=8 (no parity bit)
	USART3->CR1 &= ~(0b1 << 12);
	USART3->CR2 &= ~(0b11 << 12);
	//Enable interrupts in the NVIC
	NVIC_EnableIRQ(USART3_IRQn);
	NVIC_SetPriority(USART3_IRQn, 1);
	//Set the RXNEIE bit to 1 to raise interrupt when message is received
	USART3->CR1 |= (0b1 << 5);
}

void EnvoyerUART3(char * msg) {
	// envoyer
	//Mettre en mode envoyer / clear Set the TE bit
	USART3->CR1 &= ~(0b1 << 2);
	USART3->CR1 |= 0b1 << 3;
	int i = 0;
	while (msg[i] != '\0') {
		USART3->DR = msg[i];
		i++;
		//Attendre que le DR soit vide
		char TXE = 0b0;
		while (TXE == 0b0) {
			TXE = (USART3->SR & (0b1 << 7)) >> 7;
		}
	}
	//wait until TC = 1 (transmission complete)
	char TC = 0b0;
		while (TC == 0b0) {
			TC = (USART3->SR & (0b1 << 6)) >> 6;
		}
	// Clear the TE bit, Set the RE bit
	USART3->CR1 &= ~(0b1 << 3);
	USART3->CR1 |= (0b1 << 2);
}



#endif