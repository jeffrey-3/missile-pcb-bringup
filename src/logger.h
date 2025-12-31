#ifndef LOGGER_H
#define LOGGER_H

#include <stdint.h>
#include <string.h>

typedef struct {
    uint32_t counter;
    float roll;
} message_t;

typedef void (*logger_write_page_t)(uint32_t page, uint8_t *data);
typedef void (*logger_erase_sector_t)(uint16_t sector);
typedef void (*logger_write_enable_t)(void);
typedef void (*logger_write_disable_t)(void);
typedef void (*logger_read_page_t)(uint32_t page, uint8_t *data);
typedef void (*logger_delay_ms_t)(uint32_t ms);

typedef struct {
    logger_write_page_t write_page;
    logger_erase_sector_t erase_sector;
    logger_write_enable_t write_enable;
    logger_write_disable_t write_disable;
    logger_read_page_t read_page;
    logger_delay_ms_t delay_ms;
    uint16_t messages_per_page;
    uint16_t sector_erase_time;
    uint16_t write_enable_time;
    uint8_t message_index;
    uint32_t current_page;
    message_t *buffer;
} logger_t;

void logger_init(logger_t *logger);
void logger_write(logger_t *logger, message_t message);
void logger_erase(logger_t *logger, uint16_t sector);
void logger_read(logger_t *logger, uint32_t page, message_t *messages);

#endif // LOGGER_H
