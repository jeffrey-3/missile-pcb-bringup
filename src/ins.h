#ifndef INS_H
#define INS_H

#include "quaternion.h"

typedef struct {
    quat_t q;
    vec3_t pos;
    vec3_t vel;
    vec3_t acc_world;
} ins_t;

void ins_init(ins_t *ins);
void ins_update(ins_t *ins, float gx, float gy, float gz, float ax, float ay, 
    float az, float dt);

#endif /* INS_H */
