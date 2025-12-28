# Missile firmware

## Tools setup

To proceed, the following tools are required:

- ARM GCC, https://launchpad.net/gcc-arm-embedded - for compiling and linking
- GNU make, http://www.gnu.org/software/make/ - for build automation
- ST link, https://github.com/stlink-org/stlink - for flashing
- Git, https://git-scm.com/ - for downloading source code and version control
- GDB Multiarch, https://www.sourceware.org/gdb/ - for debugging ARM Cortex-M

### Setup instructions for Linux (Debian)

```sh
sudo apt -y update
sudo apt -y install gcc-arm-none-eabi make stlink-tools git gdb-multiarch
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
