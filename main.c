#include "hal.h"

static volatile uint32_t s_ticks;
void systick_handler(void) {
        s_ticks++;
}

int main(void) {
	systick_init(FREQ / 1000); // Tick every 1ms

	uint16_t tx = PIN('B', 6);
	uint16_t rx = PIN('B', 7);
	uint8_t af = 0;
	uart_init(UART1, af, rx, tx, 115200);

	uint16_t led = PIN('B', 9); // LED

	gpio_set_mode(led, GPIO_MODE_OUTPUT);  // Set LED to output mode

	uint32_t timer = 0;
	uint32_t period = 200; // 200ms
	for (;;) {
		if (timer_expired(&timer, period, s_ticks)) {
			static bool on;
			gpio_write(led, on);
			on = !on;
			uart_write_buf(UART1, "hi\r\n", 4);
		}
	}

  	return 0;
}
