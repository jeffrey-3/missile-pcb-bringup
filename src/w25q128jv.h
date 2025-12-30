#ifndef W25Q128JV
#define W25Q128JV

#include <stdint.h>
#include <stddef.h>

typedef void (*w25q128jv_spi_transfer_t)(const uint8_t *tx_buf, uint8_t *rx_buf,
    size_t len);

typedef struct {
    w25q128jv_spi_transfer_t spi_transfer;
} w25q128jv_t;

uint8_t w25q128jv_read_id(w25q128jv_t *device);

#endif // W25Q128JV
