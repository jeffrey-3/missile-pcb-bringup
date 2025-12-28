# Missile Firmware

## Flash

```
make flash
```

## Flash and Debug

1. In a seperate terminal, connect the STLink and start the OpenOCD server:

```
make openocd
```

2. In the original terminal, start GDB:

```
gdb-multiarch build/firmware.elf
```
