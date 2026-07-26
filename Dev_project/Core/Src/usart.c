#include "usart.h"

/*
 * @brief Configures PD8/PD9 as USART3 TX/RX (alternate function 7),
 * sets baud rate to 115200, and enables transmitter + receiver.
 */

void USART3_Init(void)
{
    // Enable GPIOD clock — PD8/PD9 are the USART3 TX/RX pins
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;

    // Set PD8 and PD9 to alternate function mode (10)
    GPIOD->MODER &= ~((0x3 << (8 * 2)) | (0x3 << (9 * 2)));
    GPIOD->MODER |=  ((0x2 << (8 * 2)) | (0x2 << (9 * 2)));

    // Select AF7 (USART3) for both pins in the AFR[1] (high) register
    GPIOD->AFR[1] &= ~((0xF << (0 * 4)) | (0xF << (1 * 4)));
    GPIOD->AFR[1] |=  ((0x7 << (0 * 4)) | (0x7 << (1 * 4)));

    // Enable USART3 peripheral clock
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;

    // Disable USART3 while configuring baud rate (avoids glitches)
    USART3->CR1 &= ~USART_CR1_UE;

    // Baud rate register: mantissa=22, fraction=13 -> 115200 baud
    // (derived from fCK / (16 * baud), see BRR formula notes)
    USART3->BRR = (22 << 4) | 13;

    // Enable USART3, transmitter, and receiver
    USART3->CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;  /* UE = enable, TE = Transmitter Enable, RE = Receiver enable*/
}

/**
 * @brief Sends a string one byte at a time, blocking until each byte
 * is accepted by the transmit data register (TXE flag).
 */
void UART_SendString(char *str)
{
    while (*str)
    {
        while (!(USART3->SR & USART_SR_TXE)); // wait until TX buffer is empty
        USART3->DR = *str;
        str++;
    }
}
