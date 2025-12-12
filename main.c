#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>

#define FREQ 16000000 // 16 MHz
#define BIT(x) (1UL << (x))
#define PIN(bank, num) ((((bank) - 'A') << 8) | (num))
#define PINNO(pin) (pin & 255)
#define PINBANK(pin) (pin >> 8)

static inline void spin(volatile uint32_t count) {
  while (count--) asm("nop");
}

struct rcc {
  volatile uint32_t CR, ICSCR, CFGR, PLLCFGR, RESERVED, CRRCR, 
	CIER, CIFR, CICR, IOPRSTR, AHBRSTR, APBRSTR1, APBRSTR2, 
	IOPENR, AHBRNR, APBENR1, APBENR2, IOPSMENR, AHBSMENR, 
	APBSMENR1, APBSMENR2, CCIPR, CCIPR2, BDCR, CSR;
};
#define RCC ((struct rcc *) 0x40021000) // RM0444 Table 6

struct systick {
	volatile uint32_t CTRL, LOAD, VAL, CALIB;
};
#define SYSTICK ((struct systick *) 0xe000e010)

static inline void systick_init(uint32_t ticks) {
	if ((ticks - 1) > 0xffffff) return; // Systick timer is 24 bit
	SYSTICK->LOAD = ticks - 1;
	SYSTICK->VAL = 0;
	SYSTICK->CTRL = BIT(0) | BIT(1) | BIT(2); // Enable systick
	RCC->APBENR2 |= BIT(0); // Enable SYSCFG
}

struct gpio {
	volatile uint32_t MODER, OTYPER, OSPEEDR, PUPDR, IDR, ODR, BSRR, 
		LCKR, AFR[2], BRR;
};
#define GPIO(bank) ((struct gpio *) (0x50000000 + 0x400 * (bank))) // RM0444 Table 6

enum { GPIO_MODE_INPUT, GPIO_MODE_OUTPUT, GPIO_MODE_AF, GPIO_MODE_ANALOG };

static inline void gpio_set_mode(uint16_t pin, uint8_t mode) {
	struct gpio *gpio = GPIO(PINBANK(pin)); // GPIO bank
	int n = PINNO(pin); // Pin number
  	RCC->IOPENR |= BIT(PINBANK(pin)); // Enable GPIO clock
	gpio->MODER &= ~(3U << (n * 2)); // Clear existing setting
	gpio->MODER |= (mode & 3) << (n * 2); // Set new mode
}

static inline void gpio_write(uint16_t pin, bool val) {
	struct gpio *gpio = GPIO(PINBANK(pin));
	gpio->BSRR = (1U << PINNO(pin)) << (val ? 0 : 16);
}

static inline void gpio_set_af(uint16_t pin, uint8_t af_num) {
	struct gpio *gpio = GPIO(PINBANK(pin)); // GPIO bank
	int n = PINNO(pin); // Pin number
	if (n < 9) {
		gpio->AFR[n >> 3] &= ~(15UL << ((n & 7) * 4));
		gpio->AFR[n >> 3] |= ((uint32_t) af_num) << ((n & 7) * 4);
	}
}

struct uart {
	volatile uint32_t CR1, CR2, CR3, BRR, GTPR, RTOR, RQR, 
		ISR, ICR, RDR, TDR, PRESC;
};
#define UART1 ((struct uart *) 0x40013800)

static inline void uart_init(struct uart *uart, uint8_t af, uint16_t rx, uint16_t tx, unsigned long baud) {
	if (uart == UART1) {
		RCC->APBENR2 |= BIT(14);
	}

	gpio_set_mode(tx, GPIO_MODE_AF);
	gpio_set_af(tx, af);
	gpio_set_mode(rx, GPIO_MODE_AF);
	gpio_set_af(rx, af);
	uart->CR1 = 0;                           // Disable this UART
	uart->BRR = FREQ / baud;                 // FREQ is a UART bus frequency
	uart->CR1 |= BIT(0) | BIT(2) | BIT(3);  // Set UE, RE, TE
}

static inline void uart_write_byte(struct uart *uart, uint8_t byte) {
  uart->TDR = byte;
  while ((uart->ISR & BIT(7)) == 0) spin(1); // Finished transmit when TXE is set
}

static inline void uart_write_buf(struct uart *uart, char *buf, size_t len) {
  while (len-- > 0) uart_write_byte(uart, *(uint8_t *) buf++);
}

static inline int uart_read_ready(struct uart *uart) {
  return uart->ISR & BIT(5);  // If RXNE bit is set, data is ready
}

static inline uint8_t uart_read_byte(struct uart *uart) {
  return (uint8_t) (uart->RDR & 255);
}

static volatile uint32_t s_ticks;
void SysTick_Handler(void) {
  s_ticks++;
}

// t: expiration time, prd: period, now: current time. Return true if expired
bool timer_expired(uint32_t *t, uint32_t prd, uint32_t now) {
  if (now + prd < *t) *t = 0;                    // Time wrapped? Reset timer
  if (*t == 0) *t = now + prd;                   // First poll? Set expiration
  if (*t > now) return false;                    // Not expired yet, return
  *t = (now - *t) > prd ? now + prd : *t + prd;  // Next expiration time
  return true;                                   // Expired, return true
}

int main(void) {
	systick_init(16000000 / 1000); // Tick every 1ms
	
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

// Startup code
__attribute__((naked, noreturn)) void _reset(void) {
	// memset .bss to zero, and copy .data section to RAM region
	extern long _sbss, _ebss, _sdata, _edata, _sidata;
	for (long *dst = &_sbss; dst < &_ebss; dst++) *dst = 0;
	for (long *dst = &_sdata, *src = &_sidata; dst < &_edata;) *dst++ = *src++;

	main(); // Call main()
	for (;;) (void) 0; // Infinite loop in the case if main() returns
}

extern void _estack(void); // Defined in link.ld

__attribute__((section(".vectors"))) void (*const tab[16 + 32])(void) = {
	_estack, _reset, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, SysTick_Handler};
