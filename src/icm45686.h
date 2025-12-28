#ifndef ICM45686_H
#define ICM45686_H

#include <stdint.h>
#include <stddef.h>

typedef void (*icm45686_spi_transfer_t)(const uint8_t *tx_buf,
                                        uint8_t *rx_buf,
                                        size_t len);

typedef struct {
    icm45686_spi_transfer_t spi_transfer;
} icm45686_t;

#define ICM45686_WHO_AM_I 0x72

uint8_t icm45686_read_id(icm45686_t *device);

#endif /* ICM45686_H */
