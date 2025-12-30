#include "logger.h"

void logger_init(logger_t *logger, uint16_t page_length,
    uint16_t sector_erase_time) {
    logger->messages_per_page = page_length / sizeof(message_t);
    logger->sector_erase_time = sector_erase_time;
    logger->message_index = 0;
    logger->current_page = 0;
}

void logger_write(logger_t *logger, message_t message) {
    logger->buffer[logger->message_index] = message;
    logger->message_index++;

    if (logger->message_index == logger->messages_per_page) {
        logger->write_page(logger->current_page, (uint8_t *)logger->buffer);
        logger->current_page++;
        logger->message_index = 0;
    }
}
