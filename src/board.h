#ifndef BOARD_H
#define BOARD_H

#include "hal.h"

typedef struct {
    uint16_t led;
    uint16_t uart1_tx;
    uint16_t uart1_rx;
    uint16_t spi1_cs;
    uint16_t spi1_miso;
    uint16_t spi1_mosi;
    uint16_t spi1_sck;
    uint16_t spi2_cs;
    uint16_t spi2_miso;
    uint16_t spi2_mosi;
    uint16_t spi2_sck;
} board_pins_t;

extern const board_pins_t board_pins;

void board_init(void);
void board_setup_led(void);
void board_setup_uart1(void);
void board_setup_spi1(void);
void board_setup_spi2(void);
void board_icm45686_spi_transfer(const uint8_t *tx_buf, uint8_t *rx_buf,
    size_t len);
void board_w25q128jv_spi_transfer(const uint8_t *tx_buf, uint8_t *rx_buf,
    size_t len);

#endif // BOARD_H
