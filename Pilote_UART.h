#ifndef __piloteUART_h
#define __piloteUART_h

#include "stm32f10x.h"

void InitUART(USART_TypeDef *UART);
char RecevoirUART(USART_TypeDef *UART);
void EnvoyerUART(USART_TypeDef *UART, char MESSAGE);



#endif