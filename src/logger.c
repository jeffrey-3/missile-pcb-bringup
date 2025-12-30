#include "logger.h"

// Need to fix to do floor rounding
static uint8_t messages_per_page = PAGE_LENGTH / sizeof(message_t);
static message_t buffer[messages_per_page];

void logger_init(logger_t *logger) {
    logger->message_index = 0;
    logger->page = 0;
}

void logger_write(logger_t *logger, message_t message) {
    buffer[logger->message_index] = message;
    logger->message_index++;

    if (logger->message_index == messages_per_page) {
        logger->write_page(logger->page, (uint8_t *)buffer);
        logger->page++;
        logger->message_index = 0;
    }
}
