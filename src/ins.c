#include "ins.h"

void ins_init(ins_t *ins) {
    ins->q = quat_identity();
    ins->vel.x = 0.0f;
    ins->vel.y = 0.0f;
    ins->vel.z = 0.0f;
    ins->pos.x = 0.0f;
    ins->pos.y = 0.0f;
    ins->pos.z = 0.0f;
    ins->first_update = true;
}

void ins_update(ins_t *ins, float gx, float gy, float gz, float ax, float ay,
    float az, float dt) {
    if (ins->first_update) {
        float roll = atan2f(-ay, -az);
        float pitch = atan2f(ax, sqrtf(ay*ay + az*az));
        ins->q = quat_from_euler(roll, pitch, 0.0f);
        ins->first_update = false;
    }

    quat_t dq;
    dq.w = 1.0f;
    dq.x = 0.5f * gx * dt;
    dq.y = 0.5f * gy * dt;
    dq.z = 0.5f * gz * dt;

    ins->q = quat_mul(ins->q, dq);
    quat_normalize(&ins->q);

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
