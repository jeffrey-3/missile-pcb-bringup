#include "ins.h"

void ins_init(ins_t *ins) {
    ins->q = quat_identity();
}

void ins_update(ins_t *ins, float gx, float gy, float gz, float ax, float ay,
    float az, float dt) {
    quat_t dq;
    dq.w = 1.0f;
    dq.x = 0.5f * gx * dt;
    dq.y = 0.5f * gy * dt;
    dq.z = 0.5f * gz * dt;

    ins->q = quat_mul(ins->q, dq);
    quat_normalize(&ins->q);

    ax = ax;
    ay = ay;
    az = az;
}
