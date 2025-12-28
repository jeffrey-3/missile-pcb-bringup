#ifndef ICM45686_H
#define ICM45686_H

#include <stdint.h>
#include <stddef.h>

#define ICM45686_WHO_AM_I 0x72
#define ICM45686_PWR_MGMT 0x10
#define ICM45686_ACCEL_DATA 0x00
#define ICM45686_GYRO_DATA 0x06
#define ICM45686_ACCEL_CONFIG0 0x1B
#define ICM45686_GYRO_CONFIG0 0x1C

typedef void (*icm45686_spi_transfer_t)(const uint8_t *tx_buf, uint8_t *rx_buf,
    size_t len);

typedef struct {
    icm45686_spi_transfer_t spi_transfer;
} icm45686_t;

void icm45686_init(icm45686_t *device);
uint8_t icm45686_read_id(icm45686_t *device);
void icm45686_read_accel(icm45686_t *device, float *data);
void icm45686_read_gyro(icm45686_t *device, float *data);

#endif /* ICM45686_H */
