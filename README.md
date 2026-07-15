# STM32 Tetris

Tetris running on a bare-metal STM32F0, rendered over UART to a serial terminal. No HAL, no OS — pure register-level C.

## Hardware

| Thing | Detail |
|---|---|
| MCU | STM32F051 (Cortex-M0, 8 MHz HSI) |
| UART | USART2 on PA2 (TX) / PA3 (RX), 115200 baud |
| LED | PA5 (onboard) |

## How it works

- **Rendering** — the board is drawn as ASCII art over UART using ANSI escape codes to reposition the cursor each frame, so the board redraws in place without scrolling
- **Input** — USART2 RX runs interrupt-driven with a ring buffer; `a`/`d` move the piece left/right
- **Gravity** — SysTick generates a 1 ms tick; the main loop checks `millis()` and drops the piece one row every 500 ms
- **No stdlib** — compiled with `-nostdlib`, no heap, no printf

## Project structure

```
main.c       game loop, input handling, gravity
board.c/h    10×20 board state
piece.c/h    piece struct, collision, rendering, movement
uart.c/h     USART2 driver (interrupt-driven RX, polling TX)
led.c/h      PA5 LED driver
systick.c/h  SysTick 1 ms tick, millis()
startup.c    vector table, Reset_Handler
linker.ld    memory layout (64K Flash, 8K SRAM)
```

## Building

Requires `arm-none-eabi-gcc` and `st-flash`.

```bash
make        # build tetris.bin
make flash  # flash via ST-Link
```


