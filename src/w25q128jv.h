#ifndef W25Q128JV_H
#define W25Q128JV_H

#include <stdint.h>
#include <stddef.h>

#define W25Q128JV_DEVICE_ID 0x90
#define W25Q128JV_READ_DATA 0x03
#define W25Q128JV_SECTOR_ERASE 0x20
#define W25Q128JV_WRITE_ENABLE 0x06
#define W25Q128JV_WRITE_DISABLE 0x04

typedef void (*w25q128jv_spi_transfer_t)(const uint8_t *tx_buf, uint8_t *rx_buf,
    size_t len);
typedef void (*w25q128jv_delay_ms_t)(uint32_t ms);

typedef struct {
    w25q128jv_spi_transfer_t spi_transfer;
    w25q128jv_delay_ms_t delay_ms;
} w25q128jv_t;

uint8_t w25q128jv_read_id(w25q128jv_t *device);
void w25q128jv_read(w25q128jv_t *device, uint32_t start_page, uint8_t offset,
    uint32_t size, uint8_t *data);
void w25q128jv_write_enable(w25q128jv_t *device);
void w25q128jv_write_disable(w25q128jv_t *device);
void w25q128jv_erase_sector(w25q128jv_t *device, uint16_t sector);

#endif // W25Q128JV_H
