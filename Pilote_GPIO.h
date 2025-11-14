#ifndef __piloteGPIO_h
#define __piloteGPIO_h

#include "stm32f10x.h"

// We use the 4 bits used in the registers.
// Example: Output (2 MHz) pushpull is 0b0010, ie 2
#define OUTPUT_PUSHPULL            0x00000002
#define OUTPUT_ALTERNATE_PUSHPULL  0x0000000A
#define OUTPUT_OPENDRAIN           0x00000006
#define INPUT_FLOATING             0x00000004
#define INPUT_ANALOG               0x00000000
#define INPUT_PULLUP               0x00000018
#define INPUT_PULLDOWN             0x00000008

void InitGPIO(GPIO_TypeDef *PORT, char BROCHE, char CONFIG);
char LireBroche(GPIO_TypeDef *PORT, char BROCHE);
void SetBroche(GPIO_TypeDef *PORT, char BROCHE);
void ResetBroche(GPIO_TypeDef *PORT, char BROCHE);

#endif