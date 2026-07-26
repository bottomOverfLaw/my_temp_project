#include "gpio.h"

void GPIO_Init(void){

	// Enable clock to GPIOB — required before configuring PB0
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

	// Clear PB0's mode bits
	GPIOB->MODER &= ~(0x3 << (0*2));

	// Set PB0 to output mode, so it can drive the onboard LED
	GPIOB->MODER |= (0x1 << (0*2));
}
