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
 * @param offset The offset in the  page from 0 to 255
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

/*
 * @brief Enable write
 *
 * Device may be unresponsive and ignore instructions for
 * up to 5 ms after use according to section 7.1.1 in datasheet
 */
void w25q128jv_write_enable(w25q128jv_t *device) {
    uint8_t tx_buf[1] = {W25Q128JV_WRITE_ENABLE};
    uint8_t rx_buf[1];
    device->spi_transfer(tx_buf, rx_buf, 1);
}

/*
 * @brief Disable write
 *
 * Device may be unresponsive and ignore instructions for
 * up to 5 ms after use according to section 7.1.1 in datasheet
 */
void w25q128jv_write_disable(w25q128jv_t *device) {
    uint8_t tx_buf[1] = {W25Q128JV_WRITE_DISABLE};
    uint8_t rx_buf[1];
    device->spi_transfer(tx_buf, rx_buf, 1);
}

/*
 * @brief Erase a sector
 *
 * Must enable write before running. Device may be unresponsive and
 * ignore instructions for up to 400 ms after use according to
 * section 7.1.1 and 9.6 in datasheet
 */
void w25q128jv_erase_sector(w25q128jv_t *device, uint16_t sector) {
    // Each sector has 16 pages of 256 bytes each
    uint32_t mem_addr = sector * 16 * 256;
    uint8_t tx_buf[4] = {W25Q128JV_SECTOR_ERASE, (mem_addr >> 16) & 0xFF,
        (mem_addr >> 8) & 0xFF, mem_addr & 0xFF};
    uint8_t rx_buf[4];

    device->spi_transfer(tx_buf, rx_buf, 4);
}

/*
 * @brief Write data to memory
 *
 * Must enable write and erase the sector before running. Device may be
 * unresponsive and ignore instructions for up to 5 ms after use according to
 * section 7.1.1 and 9.6 in datasheet (page program time)
 *
 * @param device The device handle
 * @param page The page to write on
 * @param offset The offset on the page
 * @param size The size of data to write
 * @param data The pointer to the array of data to write
 */
void w25q128jv_write_page(w25q128jv_t *device, uint32_t page, uint16_t offset,
    uint32_t size, uint8_t *data) {
    uint32_t mem_addr = page * 256 + offset;

    uint8_t tx_buf[size + 4];
    uint8_t rx_buf[size + 4];

    tx_buf[0] = W25Q128JV_PAGE_PROGRAM;
    tx_buf[1] = (mem_addr >> 16) & 0xFF;
    tx_buf[2] = (mem_addr >> 8) & 0xFF;
    tx_buf[3] = mem_addr & 0xFF;

    for (uint16_t i = 0; i < size; i++) {
        tx_buf[i + 4] = data[i];
    }

    device->spi_transfer(tx_buf, rx_buf, size + 4);
}
