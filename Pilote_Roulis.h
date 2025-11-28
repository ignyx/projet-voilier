#ifndef __piloteRoulis_h
#define __piloteRoulis_h

#include "stm32f10x.h"

// Setup SPI connection
void Roulis_init ( SPI_TypeDef * spi ) ;

// Returns 1 if abs(angle) > 45 deg.
// MUST be called after Roulis_init
int Roulis_isBoatInBadShape();

#endif