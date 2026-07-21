TARGET   = tetris
CC       = arm-none-eabi-gcc
OBJCOPY  = arm-none-eabi-objcopy
SIZE     = arm-none-eabi-size

MCU      = -mcpu=cortex-m0 -mthumb

CFLAGS   = $(MCU) -O2 -Wall -Wextra \
           -ffunction-sections -fdata-sections \
           -ffreestanding -nostdlib

LDFLAGS  = $(MCU) -T linker.ld \
           -Wl,--gc-sections \
           -Wl,-Map=$(TARGET).map \
           -nostdlib -lgcc

SRCS = startup.c main.c uart.c led.c systick.c board.c piece.c game.c rng.c
OBJS     = $(SRCS:.c=.o)

.PHONY: all flash clean

all: $(TARGET).bin
	$(SIZE) $(TARGET).elf

$(TARGET).elf: $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

# Flash via ST-Link (requires stlink-tools: apt install stlink-tools)
flash: $(TARGET).bin
	st-flash write $(TARGET).bin 0x08000000

# Alternative: flash via OpenOCD
flash-ocd: $(TARGET).elf
	openocd -f interface/stlink.cfg \
	        -f target/stm32f0x.cfg \
	        -c "program $(TARGET).elf verify reset exit"

clean:
	rm -f $(OBJS) $(TARGET).elf $(TARGET).bin $(TARGET).map
