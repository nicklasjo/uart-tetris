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
           -nostdlib

SRCS = startup.c main.c uart.c led.c systick.c board.c piece.c game.c rng.c score.c
OBJS     = $(SRCS:.c=.o)

.PHONY: all flash clean

all: $(TARGET).bin
	$(SIZE) $(TARGET).elf

# -lgcc must come after the object files: ld resolves a static archive in one
# left-to-right pass, so if it's placed before $^ it gets scanned before any
# of our undefined symbols (e.g. soft-float helpers) are even known about.
$(TARGET).elf: $(OBJS)
	$(CC) $(LDFLAGS) $^ -lgcc -o $@

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
