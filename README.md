# STM32 Tetris

Tetris running on a bare-metal STM32F030R8 Nucleo board, rendered over UART to a serial terminal. No HAL, no OS — pure register-level C.

## Hardware

| Thing | Detail |
|---|---|
| Board | STM32F030R8 Nucleo |
| MCU | STM32F030R8 (Cortex-M0, 8 MHz HSI) |
| Flash / SRAM | 64K / 8K |
| UART | USART2 on PA2 (TX) / PA3 (RX), 115200 baud |
| LED | PA5 (onboard Nucleo LED) |

## How it works

- **Rendering** — the board is drawn as ASCII art over UART using ANSI escape codes to reposition the cursor each frame, so the board redraws in place without scrolling
- **Input** — USART2 RX runs interrupt-driven with a ring buffer; `a`/`d` move the piece left/right
- **Gravity** — SysTick generates a 1 ms tick; the main loop checks `millis()` and drops the piece one row every 500 ms
- **Line clearing** — completed rows are detected and cleared, with everything above shifting down
- **No stdlib** — compiled with `-nostdlib`, no heap, no printf

## Project structure

```
main.c       game loop, input handling, gravity, line clear
board.c/h    10×20 board state, cell occupation
piece.c/h    piece struct, collision detection, rendering, movement
game.c/h     line detection and clearing logic
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
make flash  # flash via ST-Link (built into the Nucleo)
```

## Playing

Connect over serial at 115200 baud (e.g. `screen /dev/ttyACM0 115200`), then press Enter to start.

| Key | Action |
|---|---|
| `a` | Move left |
| `d` | Move right |
