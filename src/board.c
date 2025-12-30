#include "board.h"

const board_pins_t board_pins = {
    .led = PIN('B', 9),
    .tx = PIN('B', 6),
    .rx = PIN('B', 7),
    .cs = PIN('A', 1),
    .miso = PIN('A', 6),
    .mosi = PIN('A', 12),
    .sck = PIN('A', 5),
    .spi2_cs = PIN('C', 15),
    .spi2_miso = PIN('B', 2),
    .spi2_mosi = PIN('A', 4),
    .spi2_sck = PIN('A', 0)
};

void board_init(void) {
    systick_init();

    board_setup_led();
    board_setup_uart();
    board_setup_spi();
    board_setup_spi2();
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

void board_setup_spi2(void) {
    gpio_set_mode(board_pins.spi2_cs, GPIO_MODE_OUTPUT);
    gpio_write(board_pins.spi2_cs, true);

    gpio_set_mode(board_pins.spi2_miso, GPIO_MODE_AF);
    gpio_set_af(board_pins.spi2_miso, 1);

    gpio_set_mode(board_pins.spi2_mosi, GPIO_MODE_AF);
    gpio_set_af(board_pins.spi2_mosi, 1);

    gpio_set_mode(board_pins.spi2_sck, GPIO_MODE_AF);
    gpio_set_af(board_pins.spi2_sck, 0);

    spi_init(SPI2);
}

void board_icm45686_spi_transfer(const uint8_t *tx_buf, uint8_t *rx_buf,
    size_t len) {
    gpio_write(board_pins.cs, false);
    spi_transfer_buf(SPI1, tx_buf, rx_buf, len);
    gpio_write(board_pins.cs, true);
}

void board_w25q128jv_spi_transfer(const uint8_t *tx_buf, uint8_t *rx_buf,
    size_t len) {
    gpio_write(board_pins.spi2_cs, false);
    spi_transfer_buf(SPI2, tx_buf, rx_buf, len);
    gpio_write(board_pins.spi2_cs, true);
}
