#include "icm45686.h"

void icm45686_init(icm45686_t  *device) {
    uint8_t rx_buf[2];
    
    // Enable gyroscope and accelerometer in low noise mode
    uint8_t tx_buf[2] = {ICM45686_PWR_MGMT, 0x0F};
    device->spi_transfer(tx_buf, rx_buf, 2);

    // Set gyroscope to 200Hz data rate and 4000dps range
    tx_buf[0] = ICM45686_GYRO_CONFIG0;
    tx_buf[1] = 0x08;
    device->spi_transfer(tx_buf, rx_buf, 2);

    // Set accelerometer to 200Hz data rate and 32g range
    tx_buf[0] = ICM45686_ACCEL_CONFIG0;
    tx_buf[1] = 0x08;
    device->spi_transfer(tx_buf, rx_buf, 2);
}

uint8_t icm45686_read_id(icm45686_t *device) {
    uint8_t tx_buf[2] = {ICM45686_WHO_AM_I | 0x80, 0x00};
    uint8_t rx_buf[2];
    device->spi_transfer(tx_buf, rx_buf, 2);
    return rx_buf[1];
}

void icm45686_read_accel(icm45686_t *device, float *data) {
    uint8_t tx_buf[7] = {ICM45686_ACCEL_DATA | 0x80, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00};
    uint8_t rx_buf[7];
    device->spi_transfer(tx_buf, rx_buf, 7);

    int16_t raw_x = (int16_t)((rx_buf[2] << 8) | rx_buf[1]);
    int16_t raw_y = (int16_t)((rx_buf[4] << 8) | rx_buf[3]);
    int16_t raw_z = (int16_t)((rx_buf[6] << 8) | rx_buf[5]);

    // Sensitivity scale factor is in table 3.2 of datasheet
    data[0] = raw_x / 1024.0f;
    data[1] = raw_y / 1024.0f;
    data[2] = raw_z / 1024.0f;
}

void icm45686_read_gyro(icm45686_t *device, float *data) {
    uint8_t tx_buf[7] = {ICM45686_GYRO_DATA | 0x80, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00};
    uint8_t rx_buf[7];
    device->spi_transfer(tx_buf, rx_buf, 7);

    int16_t raw_x = (int16_t)((rx_buf[2] << 8) | rx_buf[1]);
    int16_t raw_y = (int16_t)((rx_buf[4] << 8) | rx_buf[3]);
    int16_t raw_z = (int16_t)((rx_buf[6] << 8) | rx_buf[5]);

    // Sensitivity scale factor is in table 3.1 of datasheet
    data[0] = raw_x / 8.2f;
    data[1] = raw_y / 8.2f;
    data[2] = raw_z / 8.2f;
}
