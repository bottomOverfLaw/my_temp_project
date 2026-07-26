#include "dac.h"

/**
 * @brief Configures PA4 as an analog output pin and enables the DAC peripheral (channel 1).
 * Must be called once before DAC_SetOutput() is used.
 */
void DAC_Init(void){

	// Enable clock to GPIOA — required before configuring PA4
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    // Clear PA4's mode bits
    GPIOA->MODER &= ~(0x3 << (4*2));

    // Set PA4 to analog mode, required for DAC_OUT1 to drive the pin
    GPIOA->MODER |= (0x3 << (4*2));

    // Enable clock to the DAC peripheral
    RCC->APB1ENR |= RCC_APB1ENR_DACEN;

    // Enable DAC channel 1 (bit 0 of DAC_CR)
    DAC->CR |= DAC_CR_EN1;
}

/**
 * @brief Writes a value to the DAC, producing a proportional voltage on PA4 (DAC_OUT1).
 * @param value 12-bit value (0-4095), mapped linearly to an output voltage between 0V and 3.3V.
 */
void DAC_SetOutput(uint16_t value){

	// Write value into DHR12R1; DAC auto-transfers it to the output register (DOR1) one clock cycle later, since TEN1 (trigger) is disabled
    DAC->DHR12R1 = value;
}
