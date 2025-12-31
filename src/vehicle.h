#ifndef VEHICLE_H
#define VEHICLE_H

#include "board.h"
#include "math.h"
#include "icm45686.h"
#include "w25q128jv.h"
#include "quaternion.h"
#include "ins.h"
#include "logger.h"

#define CMD_BUF_LEN 32

typedef enum {
    BOOT_MODE_FLIGHT,
    BOOT_MODE_CALIBRATE,
    BOOT_MODE_RETREIVE,
    BOOT_MODE_ERASE
} boot_mode_t;

typedef struct {
    boot_mode_t boot_mode;
    ins_t ins;
    logger_t logger;
    icm45686_t imu;
    w25q128jv_t flash;
    bool led_on;
    uint32_t led_timer;
    uint32_t ins_timer;
} vehicle_t;

void vehicle_init();
void vehicle_update();
void vehicle_update_flight();
void vehicle_update_calibrate();
void vehicle_update_retreive();
void vehicle_update_erase();
boot_mode_t vehicle_run_cli();
void vehicle_imu_init();
void vehicle_flash_init();
void vehicle_ins_init();
void vehicle_logger_init();
void vehicle_logger_write_page(uint32_t page, uint8_t *data);
void vehicle_logger_erase_sector(uint16_t sector);
void vehicle_logger_write_enable();
void vehicle_logger_write_disable();
void vehicle_logger_read();

#endif // VEHICLE_H
