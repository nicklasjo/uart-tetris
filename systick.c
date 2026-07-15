#include <stdint.h>
#include "systick.h"

#define CORTEX_BASE 0xE000E000UL
#define SYSTICK_BASE  (CORTEX_BASE + 0x10)    // just 0xE000E010, a number
#define SYSTICK_CSR (*(volatile uint32_t *)(SYSTICK_BASE + 0x00))
#define SYSTICK_RVR (*(volatile uint32_t *)(SYSTICK_BASE + 0x04))
#define SYSTICK_CVR (*(volatile uint32_t *)(SYSTICK_BASE + 0x08))
#define SYSTICK_CALIB (*(volatile uint32_t *)(SYSTICK_BASE + 0x0C))

static volatile uint32_t ms_ticks = 0;

void init_systick(void) {
    SYSTICK_RVR = 8000 - 1; // 8MHz HSI / 8000 = 1kHz → 1ms tick
    SYSTICK_CVR = 0;         // Clear current value
    SYSTICK_CSR |= (1 << 2); //Set clock source to processor clock
    SYSTICK_CSR |= (1 << 1); // Enable SysTick interrupt
    SYSTICK_CSR |= (1 << 0); // Enable SysTick
};

void SysTick_Handler(void) {
    ms_ticks++;          /* that's the whole ISR */
}

uint32_t millis(void) {
    return ms_ticks;
}

