#ifndef LOGGER_H
#define LOGGER_H

#include <stdint.h>

#define PAGE_LENGTH 256

typedef struct {
    uint32_t counter;
    float roll;
} message_t;

typedef void (*logger_write_page_t)(uint32_t page, uint8_t *data);

typedef struct {
    logger_write_page_t write_page;
    uint8_t message_index;
    uint32_t page;
} logger_t;

void logger_init(logger_t *logger);
void logger_write(logger_t *logger, message_t message);

#endif // LOGGER_H
