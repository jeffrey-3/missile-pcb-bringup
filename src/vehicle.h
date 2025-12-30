#ifndef VEHICLE_H
#define VEHICLE_H

#include "board.h"
#include "math.h"
#include "icm45686.h"
#include "w25q128jv.h"
#include "quaternion.h"
#include "ins.h"

typedef struct {
    ins_t ins;
    icm45686_t imu;
    w25q128jv_t flash;
} vehicle_t;

extern vehicle_t vehicle;

void vehicle_init();

#endif // VEHICLE_H
