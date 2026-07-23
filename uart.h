#ifndef UART_H
#define UART_H

#include <stdint.h>

void init_usart(void);
void send_char(char c);
void send_string(char *str);
void receive_char(char *c);
int try_receive_char(char *c);
void receive_string(char *buffer, uint32_t max_length);
void send_uint(uint32_t value);
void move_cursor(uint32_t row, uint32_t col);

#endif
