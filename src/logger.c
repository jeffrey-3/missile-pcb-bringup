#include "logger.h"

#define RING_BUFFER_SIZE 1024 // Must be a power of 2

static uint8_t data_buffer[RING_BUFFER_SIZE] = {0};

void logger_init(logger_t *logger) {
    logger->current_page = 0;

    ring_buffer_setup(&logger->ring_buffer, data_buffer, RING_BUFFER_SIZE);

    logger->write_enable();
    logger->delay_ms(logger->write_enable_time);
}

/*
 * @brief Write buffer to memory
 *
 * Write may take some time to complete at the hardware level, so do not call
 * again before the last write is complete
 */
void logger_write(logger_t *logger, message_t message) {
    // Add message to buffer
    uint8_t byte_array[sizeof(message)];
    memcpy(&byte_array, &message, sizeof(message));

    for (uint32_t i = 0; i < sizeof(message); i++) {
        ring_buffer_write(&logger->ring_buffer, byte_array[i]);
    }

    // Check if buffer is ready to flush
    uint32_t page_size = logger->messages_per_page * sizeof(message_t);
    if (ring_buffer_count(&logger->ring_buffer) > page_size) {
        // Get data from ring buffer
        uint8_t write_buf[page_size];

        for (uint32_t i = 0; i < page_size; i++) {
            ring_buffer_read(&logger->ring_buffer, &write_buf[i]);
        }
        
        // Write the data
        logger->write_page(logger->current_page, write_buf);
        logger->current_page++;
    } else {
        // After every write, the flash chip disables write, so must re-enable
        logger->write_enable();
    }
}

/*
 * @brief Erase a sector in memory
 */
void logger_erase(logger_t *logger, uint16_t sector) { 
    // After every erase, the flash chip disables write, so must re-enable
    logger->write_enable();
    logger->delay_ms(logger->write_enable_time);

    logger->erase_sector(sector);
    logger->delay_ms(logger->sector_erase_time);
}

/*
 * @brief Read one page and get message structs
 *
 * This function is very slow but temporary and not used in main loop anyways
 */
void logger_read(logger_t *logger, uint32_t page, message_t *messages) {
    logger->write_disable();
    logger->delay_ms(logger->write_enable_time);

    uint32_t size = logger->messages_per_page * sizeof(message_t);
    uint8_t data[size];
    logger->read_page(page, data);

    for (uint32_t i = 0; i < logger->messages_per_page; i++) {
        memcpy(&messages[i], &data[i * sizeof(message_t)], sizeof(message_t));
    }

    logger->write_enable();
    logger->delay_ms(logger->write_enable_time);
}
