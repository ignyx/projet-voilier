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
	USART3->CR1 |= (0x01 << 13);
	//Baud rate = 9600 / USARTDIV = 468.75 / mantissa=0x1D4 & fraction=0xC
	USART3->BRR = 0x1D4C;
	NVIC_EnableIRQ(USART3_IRQn);
	NVIC_SetPriority(USART3_IRQn, 1);
	//Set the RXNEIE bit to 1 to raise interrupt when message is received
	USART3->CR1 |= (0b1 << 5);
}

void USART3_IRQHandler() {
	//Recevoir le degré de rotation
	// Bouger le plateau

}

void EnvoyerUART(USART_TypeDef *UART, char MESSAGE) {
	// envoyer


	//Remettre en mode recevoir	
	char TXE = 0b0;
	while (TXE == 0b0) {
		TXE = (USART3->SR & (0b1 << 7)) >> 7;
	}
	// Set the RE bit USART_CR1.
	USART3->CR1 |= (0b1 << 2);
}



#endif