#include "w25q128jv.h"

uint8_t w25q128jv_read_id(w25q128jv_t *device) {
    uint8_t tx_buf[6] = {W25Q128JV_DEVICE_ID, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t rx_buf[6];
    device->spi_transfer(tx_buf, rx_buf, 6);
    return rx_buf[4];
}

/*
 * @brief Read data from the memory
 * 
 * @param device The device handle
 * @param start_page The page number to start the read from 0 to 65525
 * @param offset The offset in the start page from 0 to 255
 * @param size The number of bytes to read
 * @param data Pointer to the array the received data will be stored
 */
void w25q128jv_read(w25q128jv_t *device, uint32_t start_page, uint8_t offset,
    uint32_t size, uint8_t *data) {
    uint32_t mem_addr = (start_page * 256) + offset;
    
    uint8_t tx_buf[size + 4];
    uint8_t rx_buf[size + 4];
    
    for (uint32_t i = 0; i < size + 4; i++) {
        tx_buf[i] = 0;
    }

    tx_buf[0] = W25Q128JV_READ_DATA;
    tx_buf[1] = (mem_addr >> 16) & 0xFF;
    tx_buf[2] = (mem_addr >> 8) & 0xFF;
    tx_buf[3] = mem_addr & 0xFF;
    
    device->spi_transfer(tx_buf, rx_buf, size + 4);

    for (uint32_t i = 0; i < size; i++) {
        data[i] = rx_buf[i + 4];
    }
}
