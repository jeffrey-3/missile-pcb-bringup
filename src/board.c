#include "board.h"

const board_pins_t board_pins = {
    .led = PIN('B', 9),
    .tx = PIN('B', 6),
    .rx = PIN('B', 7),
    .cs = PIN('A', 1),
    .miso = PIN('A', 6),
    .mosi = PIN('A', 12),
    .sck = PIN('A', 5)
};

void board_init(void) {
    board_setup_led();
    board_setup_uart();
    board_setup_spi();
}

void board_setup_led(void) {
    gpio_set_mode(board_pins.led, GPIO_MODE_OUTPUT);
}

void board_setup_uart(void) {
    gpio_set_mode(board_pins.tx, GPIO_MODE_AF);
    gpio_set_af(board_pins.tx, 0);

    gpio_set_mode(board_pins.rx, GPIO_MODE_AF);
    gpio_set_af(board_pins.rx, 0);

    uart_init(UART1, 115200);
}

void board_setup_spi(void) {
    gpio_set_mode(board_pins.cs, GPIO_MODE_OUTPUT);
    gpio_write(board_pins.cs, true);

    gpio_set_mode(board_pins.miso, GPIO_MODE_AF);
    gpio_set_af(board_pins.miso, 0);

    gpio_set_mode(board_pins.mosi, GPIO_MODE_AF);
    gpio_set_af(board_pins.mosi, 0);

    gpio_set_mode(board_pins.sck, GPIO_MODE_AF);
    gpio_set_af(board_pins.sck, 0);

    spi_init(SPI1);
}
