#include "ins.h"

void ins_init(ins_t *ins) {
    ins->q = quat_identity();
    ins->vel.x = 0.0f;
    ins->vel.y = 0.0f;
    ins->vel.z = 0.0f;
    ins->pos.x = 0.0f;
    ins->pos.y = 0.0f;
    ins->pos.z = 0.0f;
    ins->acc_sum.x = 0.0f;
    ins->acc_sum.y = 0.0f;
    ins->acc_sum.z = 0.0f;
    ins->state = INS_STATE_ALIGN;
}

void ins_update(ins_t *ins, float gx, float gy, float gz, float ax, float ay,
    float az, float dt) {
    gx -= GYRO_OFF_X;
    gy -= GYRO_OFF_Y;
    gz -= GYRO_OFF_Z;

    switch (ins->state) {
        case INS_STATE_ALIGN:
            ins_align_update(ins, ax, ay, az);
            break;
        case INS_STATE_RUNNING:
            ins_attitude_update(ins, gx, gy, gz, dt);
            ins_position_update(ins, ax, ay, az, dt);
            break;
    }
}

void ins_align_update(ins_t *ins, float ax, float ay, float az) {
    ins->acc_sum.x += ax;
    ins->acc_sum.y += ay;
    ins->acc_sum.z += az;
    ins->acc_count++;

    if (ins->acc_count > INS_ALIGN_SAMPLES) {
        float ax_avg = ins->acc_sum.x / (float)ins->acc_count;
        float ay_avg = ins->acc_sum.y / (float)ins->acc_count;
        float az_avg = ins->acc_sum.z / (float)ins->acc_count;

        float roll = atan2f(-ay_avg, -az_avg);
        float pitch = atan2f(ax_avg, sqrtf(ay_avg*ay_avg + az_avg*az_avg));

        ins->q = quat_from_euler(roll, pitch, 0.0f);
        ins->state = INS_STATE_RUNNING;
    }
}

void ins_attitude_update(ins_t *ins, float gx, float gy, float gz, float dt) {
    quat_t dq;
    dq.w = 1.0f;
    dq.x = 0.5f * gx * dt;
    dq.y = 0.5f * gy * dt;
    dq.z = 0.5f * gz * dt;

    ins->q = quat_mul(ins->q, dq);
    quat_normalize(&ins->q);
}

void ins_position_update(ins_t *ins, float ax, float ay, float az, float dt) {
    vec3_t acc_body = {ax * 9.81f, ay * 9.81f, az * 9.81f};
    ins->acc_world = quat_rotate_vector(ins->q, acc_body);
    ins->acc_world.z += 9.81f;

    ins->vel.x += ins->acc_world.x * dt;
    ins->vel.y += ins->acc_world.y * dt;
    ins->vel.z += ins->acc_world.z * dt;

    ins->pos.x += ins->vel.x * dt;
    ins->pos.y += ins->vel.y * dt;
    ins->pos.z += ins->vel.z * dt;
}
