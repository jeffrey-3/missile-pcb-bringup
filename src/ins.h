#ifndef INS_H
#define INS_H

#include <stdint.h>
#include <stdbool.h>
#include "quaternion.h"

#define INS_ALIGN_SAMPLES 100

typedef enum {
    INS_STATE_ALIGN,
    INS_STATE_RUNNING
} ins_state_t;

typedef struct {
    ins_state_t state;
    quat_t q;
    vec3_t pos;
    vec3_t vel;
    vec3_t acc_world;
    vec3_t acc_sum;
    uint16_t acc_count;
} ins_t;

void ins_init(ins_t *ins);
void ins_update(ins_t *ins, float gx, float gy, float gz, float ax, float ay,
    float az, float dt);
void ins_align_update(ins_t *ins, float ax, float ay, float az);
void ins_attitude_update(ins_t *ins, float gx, float gy, float gz, float dt);
void ins_position_update(ins_t *ins, float ax, float ay, float az, float dt);

#endif /* INS_H */
