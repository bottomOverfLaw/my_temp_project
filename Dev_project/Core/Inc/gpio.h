#ifndef GPIO_H
#define GPIO_H

#include "main.h"

/**
 * @brief Configures PB0 (onboard LED) as a digital output.
 * Used as a simple heartbeat indicator to confirm the main loop is running.
 */
void GPIO_Init(void);

#endif
