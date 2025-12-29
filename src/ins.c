#include "ins.h"

void ins_init(ins_t *ins) {
    ins->q = quat_identity();
}

void ins_update(ins_t *ins, float gx, float gy, float gz, float ax, float ay,
    float az, float dt) {
    quat_update_gyro(&ins->q, gx, gy, gz, dt);
    ax = ax;
    ay = ay;
    az = az;
}
