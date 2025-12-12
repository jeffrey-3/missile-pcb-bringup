openocd -f /usr/share/openocd/scripts/interface/stlink.cfg -f /usr/share/openocd/scripts/target/stm32g0x.cfg

gdb-multiarch ./firmware.elf

target extended-remote localhost:3333

cu -l /dev/ttyACM0 -s 115200
enter ~ . to exit
