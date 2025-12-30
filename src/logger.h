#ifndef LOGGER_H
#define LOGGER_H

#include <stdint.h>

typedef struct {
    uint32_t counter;
    float roll;
} message_t;

typedef void (*logger_write_page_t)(uint32_t page, uint8_t *data);

typedef struct {
    logger_write_page_t write_page;
    uint16_t messages_per_page;
    uint16_t sector_erase_time;
    uint8_t message_index;
    uint32_t current_page;
    message_t *buffer;
} logger_t;

void logger_init(logger_t *logger, uint16_t page_length,
    uint16_t sector_erase_time);
void logger_write(logger_t *logger, message_t message);

#endif // LOGGER_H
