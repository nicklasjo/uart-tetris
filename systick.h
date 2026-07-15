#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>

void init_systick(void);
uint32_t millis(void);

#endif