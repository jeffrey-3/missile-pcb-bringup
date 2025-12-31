#include "logger.h"

void logger_init(logger_t *logger) {
    logger->message_index = 0;
    logger->current_page = 2; // Set to 2 to see if pages other than 1 works...
                            // YES IT DOES WORK AND READ WORKS
                            // But the following pages don't work!
                        // Maybe because I need write_disable()?
                       // According to datasheet, right after write it sets 
                        // write enable to zero!!!! so I need to enable again
        
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
    logger->buffer[logger->message_index] = message;
    logger->message_index++;

    if (logger->message_index == logger->messages_per_page) {
        // First page works, second page doesn't work?
        // Viewing the uint8_t data array in read with GDB, its 0xFF
        // Maybe after every write I need to write_disable or something??? no
        logger->write_page(logger->current_page, (uint8_t *)logger->buffer);
        logger->current_page++;
        logger->message_index = 0;
    }
}

/*
 * @brief Erase a sector in memory
 */
void logger_erase(logger_t *logger, uint16_t sector) {
    logger->erase_sector(sector);
    logger->delay_ms(logger->sector_erase_time);
}
#pragma GCC push_options
#pragma GCC optimize ("O0")
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
#pragma GCC pop_options
