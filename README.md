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
- **Pieces** — all 7 tetrominoes (I/O/T/S/Z/J/L) live in a static 4×4 shape table; a new one is picked at random each spawn
- **Rotation** — 90° clockwise rotation of the 4×4 shape grid via index remap (`new[r][c] = old[3-c][r]`), no rotation-matrix multiply or trig needed
- **Input** — USART2 RX runs interrupt-driven with a ring buffer; `a`/`d` move the piece left/right, `space` rotates, `s` hard-drops it to the bottom
- **Gravity** — SysTick generates a 1 ms tick; the main loop checks `millis()` and drops the piece one row every 500 ms
- **Line clearing** — completed rows are detected and cleared
- **No stdlib** — compiled with `-nostdlib`/`-ffreestanding`: no heap, no printf, and no `rand()`/`%` (this target has no hardware divider and no linked division routine), so piece selection uses a small xorshift PRNG (`rng.c/h`) built entirely from shifts/XOR/AND

## Project structure

```
main.c       game loop, input handling, gravity, line clear
board.c/h    10×20 board state, cell occupation
piece.c/h    piece struct, tetromino shapes, collision detection, rendering, movement, rotation
game.c/h     line detection and clearing logic
rng.c/h      xorshift PRNG for random piece selection (division-free)
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
| `space` | Rotate clockwise |
| `s` | Hard drop |
