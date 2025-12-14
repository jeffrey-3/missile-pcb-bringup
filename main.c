#include "hal.h"

void test_uart() {
	uint16_t tx = PIN('B', 6);
	gpio_set_mode(tx, GPIO_MODE_AF);
	gpio_set_af(tx, 0);

	uint16_t rx = PIN('B', 7);
	gpio_set_mode(rx, GPIO_MODE_AF);
	gpio_set_af(tx, 0);
	
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

void test_imu() {
        // UART
	uint16_t tx = PIN('B', 6);
	gpio_set_mode(tx, GPIO_MODE_AF);
	gpio_set_af(tx, 0);

	uint16_t rx = PIN('B', 7);
	gpio_set_mode(rx, GPIO_MODE_AF);
	gpio_set_af(tx, 0);
	
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

        uint8_t data = 0; // WHO_AM_I should be 0xE9
        uint8_t reg_address = 0x72;

        uint32_t timer = 0;
        uint32_t period = 1000;
        for (;;) {
                if (timer_expired(&timer, period)) {
                        static bool on;
                        gpio_write(led, on);
                        on = !on;

                        gpio_write(cs, false);
                        spi_write(SPI1, &reg_address, 1);
                        spi_read(SPI1, &data, 1);
                        gpio_write(cs, true);
                        
                        uart_write_buf(UART1, (char *)&data, 1);
                }
        }
}

int main(void) {
        systick_init(FREQ / 1000); // Tick every 1ms

        // test_uart();	
        test_imu();

  	return 0;
}
