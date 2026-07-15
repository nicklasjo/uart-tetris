#include <stdint.h>

#define GPIOA_BASE 0x48000000
#define GPIOA_MODER (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_AFRL  (*(volatile uint32_t *)(GPIOA_BASE + 0x20))
#define GPIOA_ODR   (*(volatile uint32_t *)(GPIOA_BASE + 0x14))

#define RCC_BASE    0x40021000
#define RCC_AHBENR  (*(volatile uint32_t *)(RCC_BASE + 0x14))

void init_led() {
    RCC_AHBENR |= (1 << 17);  // Enable GPIOA clock

    // Set PA5 to output mode
    GPIOA_MODER &= ~(3 << 10);
    GPIOA_MODER |=  (1 << 10);  // PA5 → Output mode
}

void set_led_state(int state) {
    if (state) {
        GPIOA_ODR |= (1 << 5);  // Turn on LED
    } else {
        GPIOA_ODR &= ~(1 << 5); // Turn off LED
    }
}

void toggle_led() {
    GPIOA_ODR ^= (1 << 5); // Toggle LED state
}

void delay(int time) {
    volatile int t = time;
    while(t--) {}
}

void blink_led(int time) {
    set_led_state(0);  // Turn on LED
    delay(time);      // Simple delay loop
    set_led_state(1);  // Turn off LED
    delay(time);      // Simple delay loop
}