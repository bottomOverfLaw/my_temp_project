#include "adc.h"

/**
 * @brief Initializes ADC1 to read the internal temperature sensor (channel 18).
 * Enables the ADC1 clock, powers the internal temp sensor/VREF, sets a long
 * sample time suited to a slow-changing signal, and turns the ADC on.
 */
void ADC_Init(void){

	// Enable clock to ADC1 peripheral — required before any ADC1 register writes take effect
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

    // Power on the internal temp sensor/VREF circuit, which channel 18 reads from
    ADC->CCR |= ADC_CCR_TSVREFE;

    // Set channel 18's sample time to max (480 cycles) — temperature changes slowly, so a slow sample is fine and improves accuracy
    ADC1->SMPR1 |= (0x7 << 24);

    // Select channel 18 (internal temp sensor) as the 1st (and only) conversion in the sequence
    ADC1->SQR3 |= (18 << 0);

    // Turn ADC1 on — without this, none of the above configuration actually runs a conversion
    ADC1->CR2 |= ADC_CR2_ADON;
}

/**
 * @brief Triggers a single ADC1 conversion on channel 18 (internal temp sensor) and waits for it to complete.
 * @return Raw 12-bit ADC value (0-4095) representing the internal temperature sensor's output voltage.
 */
uint32_t ADC_ReadRaw(void){

	uint32_t raw_value;

	// Trigger the ADC to begin converting (SWSTART is bit 30 of CR2)
	ADC1->CR2 |= ADC_CR2_SWSTART;

	// Block here until EOC (bit 1 of SR) is set, meaning the conversion has finished
	while(!(ADC1->SR & ADC_SR_EOC));

	 // Read the conversion result. This also automatically clears the EOC flag for the next read
	raw_value = ADC1->DR;
	return raw_value;
}
