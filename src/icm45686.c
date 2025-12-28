#include "icm45686.h"

uint8_t icm45686_read_id(icm45686_t *device) {
    uint8_t tx_buf[2] = {ICM45686_WHO_AM_I | 0x80, 0x00};
    uint8_t rx_buf[2];
    device->spi_transfer(tx_buf, rx_buf, 2);
    return rx_buf[1];
}
