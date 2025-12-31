#ifndef VEHICLE_H
#define VEHICLE_H

#include "board.h"
#include "math.h"
#include "icm45686.h"
#include "w25q128jv.h"
#include "quaternion.h"
#include "ins.h"
#include "logger.h"

typedef struct {
    ins_t ins;
    logger_t logger;
    icm45686_t imu;
    w25q128jv_t flash;
} vehicle_t;

void vehicle_init();
void vehicle_update();
void vehicle_logger_init();
void vehicle_logger_write_page(uint32_t page, uint8_t *data);
void vehicle_logger_write_enable();
void vehicle_logger_write_disable();

#endif // VEHICLE_H
