#include "logger.h"

void logger_init(logger_t *logger) {
    logger->message_index = 0;
    logger->current_page = 0;

    logger->write_enable();
    logger->delay_ms(logger->write_enable_time);

    // Must erase sector before writing
    // In the future, have an erase command over CLI
    // Don't write unless erased
    // Don't need to read to check if it is erased before writing, it just
    // simply won't work when you write to the chip
    logger->erase_sector(0);
    logger->delay_ms(logger->sector_erase_time);
}

/*
 * @brief Write buffer to memory
 *
 * Write may take some time to complete at the hardware level, so do not call
 * again before the last write is complete
 */
void logger_write(logger_t *logger, message_t message) {
    logger->buffer[logger->message_index] = message;
    logger->message_index++;

    if (logger->message_index == logger->messages_per_page) {
        logger->write_page(logger->current_page, (uint8_t *)logger->buffer);
        logger->current_page++;
        logger->message_index = 0;
    }
}
