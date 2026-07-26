#ifndef ADC_H
#define ADC_H

#include "main.h"

/*
 * @brief Initializes ADC1 to read the internal temperature sensor (channel 18).
 * Enables the ADC1 clock, powers the internal temp sensor/VREF, sets a long
 * sample time suited to a slow-changing signal, and turns the ADC on.
 * */
void ADC_Init(void);

/*
 * @brief Triggers a single ADC1 conversion on channel 18 and returns the raw 12-bit result.
 * @return Raw ADC value (0-4095), representing the internal temperature sensor voltage.
 * */
uint32_t ADC_ReadRaw(void);

#endif
