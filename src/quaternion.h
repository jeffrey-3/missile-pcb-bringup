#ifndef QUATERNION_H
#define QUATERNION_H

#include <math.h>

typedef struct {
    float w;
    float x;
    float y;
    float z;
} quat_t;

typedef struct {
    float x;
    float y;
    float z;
} vec3_t;

quat_t quat_identity();
void quat_normalize(quat_t *q);
quat_t quat_mul(quat_t q1, quat_t q2);
vec3_t quat_rotate_vector(quat_t q, vec3_t v);
void quat_to_euler(quat_t q, float *roll, float *pitch, float *yaw);
quat_t quat_from_euler(float roll, float pitch, float yaw);

#endif // QUATERNION_H
