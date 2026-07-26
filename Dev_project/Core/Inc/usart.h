#ifndef USART_H
#define USART_H

#include "main.h" //pulls the definition

/*
 * @brief Initialise USART3 on PD8(TX)/ PD9(RX) at 115200 baud.
 * Bare-metal register setup
 */
void USART3_Init(void);

/*
 * @biref Blocking transmit of a null-terminated string over USART3.
 * @param str pointer to the string to send
 */
void UART_SendString(char *str);

#endif
