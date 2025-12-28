#ifndef QUATERNION_H
#define QUATERNION_H

#include <math.h>

typedef struct {
    float w;
    float x;
    float y;
    float z;
} quat_t;

quat_t quat_identity();
void quat_normalize(quat_t *q);
quat_t quat_mul(quat_t q1, quat_t q2);

#endif // QUATERNION_H
