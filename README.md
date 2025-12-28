# Missile firmware

## Tools setup

### Setup instructions for Linux (Debian)

```sh
sudo apt -y update
sudo apt -y install gcc-arm-none-eabi make stlink-tools git gdb-multiarch openocd
```

## Build

1. Clone the repository
2. Set the working directory to the repository root
3. Build the firmware:

```sh
make
```

4. The firmware will be in the `build` directory

## Flash

```sh
make flash
```

## Flash and debug

1. In a seperate terminal, connect the STLink and start the OpenOCD server:

```sh
make openocd
```

2. In the original terminal, start GDB:

```sh
gdb-multiarch build/firmware.elf
```
