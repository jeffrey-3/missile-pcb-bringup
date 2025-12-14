#ifndef HAL_H
#define HAL_H

#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>

#define FREQ 16000000 // 16 MHz
#define BIT(x) (1UL << (x))
#define PIN(bank, num) ((((bank) - 'A') << 8) | (num))
#define PINNO(pin) (pin & 255)
#define PINBANK(pin) (pin >> 8)

void _systick_handler(void);

struct rcc {
        volatile uint32_t CR, ICSCR, CFGR, PLLCFGR, RESERVED, CRRCR, CIER,
                CIFR, CICR, IOPRSTR, AHBRSTR, APBRSTR1, APBRSTR2, IOPENR,
                AHBRNR, APBENR1, APBENR2, IOPSMENR, AHBSMENR, APBSMENR1,
                APBSMENR2, CCIPR, CCIPR2, BDCR, CSR;
};
#define RCC ((struct rcc *) 0x40021000)

struct systick {
	volatile uint32_t CTRL, LOAD, VAL, CALIB;
};
#define SYSTICK ((struct systick *) 0xe000e010)

void systick_init(uint32_t ticks);
bool timer_expired(uint32_t *t, uint32_t prd);
void spin(volatile uint32_t count);

struct gpio {
	volatile uint32_t MODER, OTYPER, OSPEEDR, PUPDR, IDR, ODR, BSRR, LCKR,
                AFR[2], BRR;
};
#define GPIO(bank) ((struct gpio *) (0x50000000 + 0x400 * (bank)))

enum { GPIO_MODE_INPUT, GPIO_MODE_OUTPUT, GPIO_MODE_AF, GPIO_MODE_ANALOG };

void gpio_set_mode(uint16_t pin, uint8_t mode);
void gpio_write(uint16_t pin, bool val);
void gpio_set_af(uint16_t pin, uint8_t af_num);

struct uart {
	volatile uint32_t CR1, CR2, CR3, BRR, GTPR, RTOR, RQR, ISR, ICR, RDR,
                 TDR, PRESC;
};
#define UART1 ((struct uart *) 0x40013800)

void uart_init(struct uart *uart,
               uint8_t af,
               uint16_t rx,
               uint16_t tx,
               unsigned long baud);


void uart_write_byte(struct uart *uart, uint8_t byte);
void uart_write_buf(struct uart *uart, char *buf, size_t len);
int uart_read_ready(struct uart *uart);
uint8_t uart_read_byte(struct uart *uart);

struct spi {
        volatile uint32_t CR1, CR2, SR, DR, CRCPR, RXCRCR, TXCRCR, I2SCFGR,
                I2SPR;
};
#define SPI1 ((struct spi *) 0x40013000)

void spi_init();
void spi_write(uint16_t cs);

#endif /* HAL_H */
