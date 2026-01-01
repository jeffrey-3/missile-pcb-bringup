#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "util/ring_buffer.h"
#include "config.h"

typedef struct __attribute__((packed)) {
    uint32_t counter;
    uint32_t time;
    float gx;
    float gy;
    float gz;
    float ax;
    float ay;
    float az;
} message_t;

typedef void (*logger_write_page_t)(uint32_t page, uint8_t *data);
typedef void (*logger_erase_sector_t)(uint16_t sector);
typedef void (*logger_write_enable_t)(void);
typedef void (*logger_write_disable_t)(void);
typedef void (*logger_read_page_t)(uint32_t page, uint8_t *data);
typedef void (*logger_delay_ms_t)(uint32_t ms);
typedef void (*logger_output_callback_t)(char *str, size_t len);

typedef struct {
    logger_write_page_t write_page;
    logger_erase_sector_t erase_sector;
    logger_write_enable_t write_enable;
    logger_write_disable_t write_disable;
    logger_read_page_t read_page;
    logger_delay_ms_t delay_ms;
    logger_output_callback_t output_callback;
    uint32_t current_page;
    ring_buffer_t ring_buffer;
} logger_t;

void logger_init(logger_t *logger);
void logger_write(logger_t *logger, message_t message);
void logger_erase(logger_t *logger, uint16_t sector);
void logger_read(logger_t *logger, uint32_t page, message_t *messages);
void logger_read_output(logger_t *logger);
void logger_erase_output(logger_t *logger);

#endif // LOGGER_H
