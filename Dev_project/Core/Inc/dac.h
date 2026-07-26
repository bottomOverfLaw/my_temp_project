#ifndef DAC_H
#define DAC_H

#include "main.h"

/**
 * @brief Initializes PA4 (DAC_OUT1) in analog mode and enables the DAC peripheral.
 * Must be called once before DAC_SetOutput() is used.
 */

void DAC_Init(void);


/**
 * @brief Writes a value to the DAC, producing a proportional voltage on PA4 (DAC_OUT1).
 * @param value 12-bit value (0-4095), mapped linearly to an output voltage between 0V and 3.3V.
 */
void DAC_SetOutput(uint16_t value);


#endif
