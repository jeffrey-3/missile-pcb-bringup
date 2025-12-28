#include "hal.h"

static volatile uint32_t s_ticks;

void _systick_handler(void) {
    s_ticks++;
}

void systick_init(uint32_t ticks) {
    if ((ticks - 1) > 0xffffff) return; // Systick timer is 24 bit
    SYSTICK->LOAD = ticks - 1;
    SYSTICK->VAL = 0;
    SYSTICK->CTRL = BIT(0) | BIT(1) | BIT(2); // Enable systick
    RCC->APBENR2 |= BIT(0); // Enable SYSCFG
}

/**
 * @brief Check if timer expired
 *
 * @param t Expiration time
 * @param prd Period
 *
 * @return true Expired
 * @return false Not expired
 */
bool timer_expired(uint32_t *t, uint32_t prd) {
    if (s_ticks + prd < *t) *t = 0; // Time wrapped? Reset timer
    if (*t == 0) *t = s_ticks + prd; // First poll? Set expiration
    if (*t > s_ticks) return false; // Not expired yet, return

    // Next expiration time
    *t = (s_ticks - *t) > prd ? s_ticks + prd : *t + prd;

    return true; // Expired, return true
}

void spin(volatile uint32_t count) {
    while (count--) asm("nop");
}

void gpio_set_mode(uint16_t pin, uint8_t mode) {
    struct gpio *gpio = GPIO(PINBANK(pin)); // GPIO bank
    int n = PINNO(pin); // Pin number
    RCC->IOPENR |= BIT(PINBANK(pin)); // Enable GPIO clock
    gpio->MODER &= ~(3U << (n * 2)); // Clear existing setting
    gpio->MODER |= (mode & 3) << (n * 2); // Set new mode
}

void gpio_write(uint16_t pin, bool val) {
    struct gpio *gpio = GPIO(PINBANK(pin));
    gpio->BSRR = (1U << PINNO(pin)) << (val ? 0 : 16);
}

void gpio_set_af(uint16_t pin, uint8_t af_num) {
    struct gpio *gpio = GPIO(PINBANK(pin)); // GPIO bank
    int n = PINNO(pin); // Pin number
    gpio->AFR[n >> 3] &= ~(15UL << ((n & 7) * 4));
    gpio->AFR[n >> 3] |= ((uint32_t) af_num) << ((n & 7) * 4);
}

void uart_init(struct uart *uart, unsigned long baud) {
    if (uart == UART1) {
        RCC->APBENR2 |= BIT(14);
    }

    uart->CR1 = 0; // Disable this UART
    uart->BRR = FREQ / baud; // FREQ is a UART bus frequency
    uart->CR1 |= BIT(0) | BIT(2) | BIT(3); // Set UE, RE, TE
}

void uart_write_byte(struct uart *uart, uint8_t byte) {
    uart->TDR = byte;

    // Finished transmit when TXE is set
    while ((uart->ISR & BIT(7)) == 0) spin(1);
}

void uart_write_buf(struct uart *uart, char *buf, size_t len) {
    while (len-- > 0) uart_write_byte(uart, *(uint8_t *) buf++);
}

int uart_read_ready(struct uart *uart) {
    return uart->ISR & BIT(5); // If RXNE bit is set, data is ready
}

uint8_t uart_read_byte(struct uart *uart) {
    return (uint8_t) (uart->RDR & 255);
}

void spi_init(struct spi *spi) {
    if (spi == SPI1) {
        RCC->APBENR2 |= BIT(12);
    }

    spi->CR1 |= BIT(2); // Set as master
    spi->CR1 |= BIT(9); // Enable SSM to set NSS input manually
    spi->CR1 |= BIT(8); // Keep NSS input high so STM receives data
    spi->CR1 |= BIT(1); // Set CPOL to high when idle
    spi->CR1 |= BIT(0); // Set CPHA to 1
    spi->CR1 |= BIT(5); // Set clock speed to CPU_FREQ / 32
    spi->CR2 |= BIT(12); // Trigger RXNE at 8-bit FIFO
    spi->CR1 |= BIT(6); // SPI peripheral enable
}

uint8_t spi_transfer(struct spi *spi, uint8_t tx_data) {
    // Wait until TXE is set
    while (!(spi->SR & BIT(1))) spin(1);

    *(volatile uint8_t *)&spi->DR = tx_data;

    // Wait until RXNE is set
    while (!(spi->SR & BIT(0))) spin(1);

    return *(volatile uint8_t *)&spi->DR;
}

void spi_transfer_buf(struct spi *spi, const uint8_t *tx_buf, uint8_t *rx_buf,
    size_t len) {
    for (size_t i = 0; i < len; i++) {
        rx_buf[i] = spi_transfer(spi, tx_buf[i]);
    }
}
