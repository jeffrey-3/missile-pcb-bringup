# Missile Firmware

## Flash

```
make flash
```

## Flash and Debug

In a seperate terminal, run:

```
make openocd
```

Then in the original terminal, run:

```
gdb-multiarch build/firmware.elf
```
