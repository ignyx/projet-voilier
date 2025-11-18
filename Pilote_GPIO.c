#include "stm32f10x.h"
#include "Pilote_GPIO.h"

void InitGPIO(GPIO_TypeDef *PORT, char BROCHE, char CONFIG) {
	// Start clocks for GPIO A B and C
	RCC->APB2ENR |= (0x01 << 2) |(0x01 << 3) |(0x01 << 4);
	// NOTE: control register value is copied, not modified directly.
	// We will update it at the end of this function.
	uint32_t control = BROCHE < 8 ? PORT->CRL : PORT->CRH;
	// CRL and CRH each manage 8 pins
	const uint32_t rel_broche = BROCHE < 8 ? BROCHE : BROCHE - 8;
	// Each pin has 2 mode bits and 2 control bits
	const uint32_t bit_shift = (4 * rel_broche);
	// The last 4 bits from CONFIG are the MODE and CNF bits
	const uint32_t control_bits = CONFIG & 0x0F;
	// Set the 4 control bits to zero, then write
	control &= ~(0x0F << bit_shift);	
	control |= control_bits << bit_shift;
	
	// Write control back to memory
	if (BROCHE < 8)
		PORT->CRL = control;
	else
		PORT->CRH = control;
	
	// if in pull-up / pull-down, set the output bit to specify which one it is
	if (CONFIG == INPUT_PULLUP) {
		PORT->ODR &= ~(0x1 << BROCHE);
		PORT->ODR |= 0x1 << BROCHE;
	}
}

char LireBroche(GPIO_TypeDef *PORT, char BROCHE) {
	// assume pins are already set up correctly
	// we shift again so the bit is on the LSB, because we return a char
	return (PORT->IDR & (0x1 << BROCHE)) >> BROCHE;
}
 
void SetBroche(GPIO_TypeDef *PORT, char BROCHE) {
	PORT->ODR |= 0x1 << BROCHE;
}

void ResetBroche(GPIO_TypeDef *PORT, char BROCHE) {
	PORT->ODR &= ~(0x1 << BROCHE);
}



