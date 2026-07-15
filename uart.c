#include <stdint.h>
#include "uart.h"
#define RCC_BASE    0x40021000
#define RCC_AHBENR  (*(volatile uint32_t *)(RCC_BASE + 0x14))
#define RCC_APB1ENR (*(volatile uint32_t *)(RCC_BASE + 0x1C))

#define USART_BASE 0x40004400
#define USART_CR1  (*(volatile uint32_t *)(USART_BASE + 0x00))
#define USART_BRR  (*(volatile uint32_t *)(USART_BASE + 0x0C))
#define USART_ISR  (*(volatile uint32_t *)(USART_BASE + 0x1C))
#define USART_TDR  (*(volatile uint32_t *)(USART_BASE + 0x28))
#define USART_RDR  (*(volatile uint32_t *)(USART_BASE + 0x24))

#define GPIOA_BASE 0x48000000
#define GPIOA_MODER (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_AFRL  (*(volatile uint32_t *)(GPIOA_BASE + 0x20))

#define NVIC_ISER_BASE 0xE000E100
#define NVIC_ISER (*(volatile uint32_t *)(NVIC_ISER_BASE + 0x00))

#define RX_BUF_SIZE 64
static volatile char rx_buffer[RX_BUF_SIZE];
static volatile uint32_t rx_head = 0;
static volatile uint32_t rx_tail = 0;

void init_usart(void) {
    RCC_AHBENR  |= (1 << 17);  // Enable GPIOA clock
    RCC_APB1ENR |= (1 << 17);  // Enable USART2 clock

    // Set PA2 and PA3 to alternate function mode, then select AF7 (USART2)
    GPIOA_MODER &= ~(3 << 4);
    GPIOA_MODER |=  (2 << 4);  // PA2 → AF mode
    GPIOA_MODER &= ~(3 << 6);
    GPIOA_MODER |=  (2 << 6);  // PA3 → AF mode

    GPIOA_AFRL |= (1 << 8);   // PA2 → AF7 (USART2_TX)
    GPIOA_AFRL |= (1 << 12);  // PA3 → AF7 (USART2_RX)



    USART_BRR = 69;
    USART_CR1 |= (1 << 5);  // Enable RXNE interrupt
    NVIC_ISER |= (1 << 28);  // Enable USART2 interrupt in NVIC
    USART_CR1 |= (1 << 3);  // Enable transmitter
    USART_CR1 |= (1 << 2);  // Enable receiver
    USART_CR1 |= (1 << 0);  // Enable USART
}

void send_char(char c) {
    while (!(USART_ISR & (1 << 7)));
    USART_TDR = c;
}

void send_string(char *str) {
    while (*str) {
        send_char(*str++);

    }
}


int try_receive_char(char *c) {
    if (rx_head == rx_tail) {
        return 0;                          /* empty -- nothing to give */
    }
    *c = rx_buffer[rx_tail];               /* read the oldest unread byte */
    rx_tail = rx_tail + 1;
    if (rx_tail >= RX_BUF_SIZE) {
        rx_tail = 0;                       /* wrap, same idea as rx_head */
        }
    return 1;
}

void receive_string(char *buffer, uint32_t max_length) {
    uint32_t index = 0;
    char received_char;
    while (index < max_length - 1) {
        if (!try_receive_char(&received_char)) {
            break;
        }
        if (received_char == '\r') {
            break;
        }
        buffer[index++] = received_char;
    }
    buffer[index] = '\0';
}

void USART2_IRQHandler(void) {
    if (USART_ISR & (1 << 5)) {  // Check if RXNE flag is set
        char received_char = USART_RDR;  // Read the received character
        uint32_t next = rx_head +1;
        if (next >= RX_BUF_SIZE) {
            next = 0;  // Wrap around if we reach the end of the buffer
        }
        if (next != rx_tail) {  // Check for buffer overflow
            rx_buffer[rx_head] = received_char;  // Store the character in the buffer
            rx_head = next;  // Move head to the next position
        
        
        }
    }

}