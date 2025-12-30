CFLAGS  ?=  -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion \
            -Wformat-truncation -fno-common -Wconversion \
            -g3 -Os -ffunction-sections -fdata-sections -I. \
            -mcpu=cortex-m0plus -mthumb -mfloat-abi=soft $(EXTRA_CFLAGS)
LDFLAGS ?= -Tlink.ld -nostartfiles -nostdlib --specs nano.specs \
           -u _printf_float -lm -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map
SOURCES = src/main.c \
          src/startup.c \
          src/syscalls.c \
          src/hal.c \
          src/board.c \
          src/icm45686.c \
          src/w25q128jv.c \
          src/quaternion.c \
          src/math.c \
          src/ins.c \
          src/logger.c \
          src/vehicle.c

build: build/firmware.elf

create-build:
	mkdir -p build

build/firmware.elf: $(SOURCES) | create-build
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $@

build/firmware.bin: build/firmware.elf
	arm-none-eabi-objcopy -O binary $< $@

flash: build/firmware.bin
	st-flash --reset write $< 0x8000000

clean:
	rm -rf build

openocd:
	openocd -f /usr/share/openocd/scripts/interface/stlink.cfg \
			-f /usr/share/openocd/scripts/target/stm32g0x.cfg
