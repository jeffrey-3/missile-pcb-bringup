#include "board.h"

const board_pins_t board_pins = {
    .led = PIN('B', 9),
    .uart1_tx = PIN('B', 6),
    .uart1_rx = PIN('B', 7),
    .spi1_cs = PIN('A', 1),
    .spi1_miso = PIN('A', 6),
    .spi1_mosi = PIN('A', 12),
    .spi1_sck = PIN('A', 5),
    .spi2_cs = PIN('C', 15),
    .spi2_miso = PIN('B', 2),
    .spi2_mosi = PIN('A', 4),
    .spi2_sck = PIN('A', 0)
};

void board_init(void) {
    systick_init();

    board_setup_led();
    board_setup_uart1();
    board_setup_spi1();
    board_setup_spi2();
}

void board_setup_led(void) {
    gpio_set_mode(board_pins.led, GPIO_MODE_OUTPUT);
}

void board_setup_uart1(void) {
    gpio_set_mode(board_pins.uart1_tx, GPIO_MODE_AF);
    gpio_set_af(board_pins.uart1_tx, 0);

    gpio_set_mode(board_pins.uart1_rx, GPIO_MODE_AF);
    gpio_set_af(board_pins.uart1_rx, 0);

    uart_init(UART1, 115200);
}

void board_setup_spi1(void) {
    gpio_set_mode(board_pins.spi1_cs, GPIO_MODE_OUTPUT);
    gpio_write(board_pins.spi1_cs, true);

    gpio_set_mode(board_pins.spi1_miso, GPIO_MODE_AF);
    gpio_set_af(board_pins.spi1_miso, 0);

    gpio_set_mode(board_pins.spi1_mosi, GPIO_MODE_AF);
    gpio_set_af(board_pins.spi1_mosi, 0);

    gpio_set_mode(board_pins.spi1_sck, GPIO_MODE_AF);
    gpio_set_af(board_pins.spi1_sck, 0);

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
    gpio_write(board_pins.spi1_cs, false);
    spi_transfer_buf(SPI1, tx_buf, rx_buf, len);
    gpio_write(board_pins.spi1_cs, true);
}

void board_w25q128jv_spi_transfer(const uint8_t *tx_buf, uint8_t *rx_buf,
    size_t len) {
    gpio_write(board_pins.spi2_cs, false);
    spi_transfer_buf(SPI2, tx_buf, rx_buf, len);
    gpio_write(board_pins.spi2_cs, true);
}
