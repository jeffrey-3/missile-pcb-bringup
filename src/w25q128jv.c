#include "w25q128jv.h"

uint8_t w25q128jv_read_id(w25q128jv_t *device) {
    // Table 8.1.2 of the datasheet
    uint8_t tx_buf[6] = {0x90, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t rx_buf[6];
    device->spi_transfer(tx_buf, rx_buf, 6);
    return rx_buf[4];
}
