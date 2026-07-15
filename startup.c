#include <stdint.h>

extern uint32_t _estack;
extern uint32_t _sdata, _edata, _etext;
extern uint32_t _sbss, _ebss;
extern uint32_t _sidata;     // replace _etext

extern int main(void);

void Reset_Handler(void);
void Default_Handler(void);
void SysTick_Handler(void);
void USART2_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));

__attribute__((section(".isr_vector"), used))
const uint32_t vector_table[] = {
    [0]       = (uint32_t)&_estack,
    [1]       = (uint32_t)Reset_Handler,
    [2]       = (uint32_t)Default_Handler,   /* NMI       */
    [3]       = (uint32_t)Default_Handler,   /* HardFault */
    [11]      = (uint32_t)Default_Handler,   /* SVCall    */
    [14]      = (uint32_t)Default_Handler,   /* PendSV    */
    [15]      = (uint32_t)SysTick_Handler,   /* SysTick   */
    [16 + 28] = (uint32_t)USART2_IRQHandler, /* USART2    */
};

void Reset_Handler(void) {
    uint32_t *src = &_sidata;
    uint32_t *dst = &_sdata;
    while (dst < &_edata)
        *dst++ = *src++;

    dst = &_sbss;
    while (dst < &_ebss)
        *dst++ = 0;

    main();
    while (1);
}

void Default_Handler(void) {
    while (1);
}
