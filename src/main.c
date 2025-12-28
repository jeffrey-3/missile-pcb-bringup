#include "hal.h"

void test_uart() {
    uint16_t tx = PIN('B', 6);
    gpio_set_mode(tx, GPIO_MODE_AF);
    gpio_set_af(tx, 0);

    uint16_t rx = PIN('B', 7);
    gpio_set_mode(rx, GPIO_MODE_AF);
    gpio_set_af(rx, 0);

    uart_init(UART1, 115200);

    uint16_t led = PIN('B', 9);
    gpio_set_mode(led, GPIO_MODE_OUTPUT);

    uint32_t timer = 0;
    uint32_t period = 200; // 200ms
    for (;;) {
        if (timer_expired(&timer, period)) {
            static bool on;
            gpio_write(led, on);
            on = !on;
            uart_write_buf(UART1, "hi\r\n", 4);
        }
    }
}

void test_spi() {
    // UART
    uint16_t tx = PIN('B', 6);
    gpio_set_mode(tx, GPIO_MODE_AF);
    gpio_set_af(tx, 0);

    uint16_t rx = PIN('B', 7);
    gpio_set_mode(rx, GPIO_MODE_AF);
    gpio_set_af(rx, 0);

    uart_init(UART1, 115200);

    // LED
    uint16_t led = PIN('B', 9);
    gpio_set_mode(led, GPIO_MODE_OUTPUT);

    // SPI
    uint16_t cs = PIN('A', 1);
    gpio_set_mode(cs, GPIO_MODE_OUTPUT);
    gpio_write(cs, true);

    uint16_t miso = PIN('A', 6);
    gpio_set_mode(miso, GPIO_MODE_AF);
    gpio_set_af(miso, 0);

    uint16_t mosi = PIN('A', 12);
    gpio_set_mode(mosi, GPIO_MODE_AF);
    gpio_set_af(mosi, 0);

    uint16_t sck = PIN('A', 5);
    gpio_set_mode(sck, GPIO_MODE_AF);
    gpio_set_af(sck, 0);

    spi_init(SPI1);

    uint32_t timer = 0;
    uint32_t period = 1000;
    for (;;) {
        if (timer_expired(&timer, period)) {
            static bool on;
            gpio_write(led, on);
            on = !on;

            uint8_t tx_buf[2] = {0x72 | 0x80, 0x00};
            uint8_t rx_buf[2];

            gpio_write(cs, false);
            spi_transfer_buf(SPI1, tx_buf, rx_buf, 2);
            gpio_write(cs, true);

            char uart_buf[32];
            snprintf(uart_buf, sizeof(uart_buf), "WHO_AM_I: %d\r\n", rx_buf[1]);
            uart_write_buf(UART1, uart_buf, strlen(uart_buf));
        }
    }
}

void test_imu() {

}

int main(void) {
    systick_init(FREQ / 1000); // Tick every 1ms

    // test_uart();
    // test_spi();
    test_imu();

    return 0;
}
