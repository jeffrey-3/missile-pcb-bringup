#ifndef BOARD_H
#define BOARD_H

#include "hal.h"

typedef struct {
    uint16_t led;
    uint16_t tx;
    uint16_t rx;
    uint16_t cs;
    uint16_t miso;
    uint16_t mosi;
    uint16_t sck;
} board_pins_t;

extern const board_pins_t board_pins;

void board_init(void);
void board_setup_led(void);
void board_setup_uart(void);
void board_setup_spi(void);

#endif // BOARD_H
