CFLAGS  ?=  -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion \
            -Wformat-truncation -fno-common -Wconversion \
            -g3 -Os -ffunction-sections -fdata-sections -I. \
            -mcpu=cortex-m0plus -mthumb $(EXTRA_CFLAGS)
LDFLAGS ?= -Tlink.ld -nostartfiles -nostdlib --specs nano.specs \
           -lc -lgcc -Wl,--gc-sections -Wl,-Map=build/firmware.map
SOURCES = main.c startup.c syscalls.c hal.c imu.c

build: build/firmware.elf

create_build:
	mkdir -p build

build/firmware.elf: $(SOURCES) | create_build
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
