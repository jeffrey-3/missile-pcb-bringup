#include "logger.h"

static uint8_t data_buffer[LOGGER_RING_BUF_SIZE] = {0};

void logger_init(logger_t *logger) {
    logger->current_page = 0;

    ring_buffer_setup(&logger->ring_buffer, data_buffer, LOGGER_RING_BUF_SIZE);

    logger->write_enable();
    logger->delay_ms(LOGGER_WRITE_EN_TIME);
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
    uint32_t page_size = LOGGER_MSG_PER_PAGE * sizeof(message_t);
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
    logger->delay_ms(LOGGER_WRITE_EN_TIME);

    logger->erase_sector(sector);
    logger->delay_ms(LOGGER_SECTOR_ERASE_TIME);

    logger->write_enable();
    logger->delay_ms(LOGGER_WRITE_EN_TIME);
}

/*
 * @brief Read one page and get message structs
 *
 * This function is very slow but temporary and not used in main loop anyways
 */
void logger_read(logger_t *logger, uint32_t page, message_t *messages) {
    logger->write_disable();
    logger->delay_ms(LOGGER_WRITE_EN_TIME);

    uint32_t size = LOGGER_MSG_PER_PAGE * sizeof(message_t);
    uint8_t data[size];
    logger->read_page(page, data);

    for (uint32_t i = 0; i < LOGGER_MSG_PER_PAGE; i++) {
        memcpy(&messages[i], &data[i * sizeof(message_t)], sizeof(message_t));
    }

    logger->write_enable();
    logger->delay_ms(LOGGER_WRITE_EN_TIME);
}

void logger_read_output(logger_t *logger) {
    for (uint32_t i = 0; i < LOGGER_NUM_PAGES; i++) {
        char buf[100];
        snprintf(buf, sizeof(buf), "Reading page %ld out of %d\r\n", i + 1,
            LOGGER_NUM_PAGES);
        logger->output_callback(buf, strlen(buf));

        message_t messages[LOGGER_MSG_PER_PAGE];
        logger_read(logger, i, messages);

        for (uint8_t j = 0; j < LOGGER_MSG_PER_PAGE; j++) {
            message_t message = messages[j];

            char uart_buf[64];
            snprintf(uart_buf, sizeof(uart_buf),
                "%ld,%ld,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\r\n",
                message.counter, message.time, (double)message.gx,
                (double)message.gy, (double)message.gz, (double)message.ax,
                (double)message.ay, (double)message.az);
            logger->output_callback(uart_buf, strlen(uart_buf));
        }
    }

    logger->output_callback("Finished\r\n", strlen("Finished\r\n"));
}

void logger_erase_output(logger_t *logger) {
    for (uint16_t i = 0; i < LOGGER_NUM_SECTORS; i++) {
        logger_erase(logger, i);

        char uart_buf[100];
        snprintf(uart_buf, sizeof(uart_buf), "Erased %d out of %d\r\n",
            i + 1, LOGGER_NUM_SECTORS);
        logger->output_callback(uart_buf, strlen(uart_buf));
    }

    char uart_buf[100] = "Finished erase. Power cycle now.\r\n";
    logger->output_callback(uart_buf, strlen(uart_buf));
}
