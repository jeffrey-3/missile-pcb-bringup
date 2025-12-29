#include "quaternion.h"

quat_t quat_identity() {
    quat_t q = {1.0f, 0.0f, 0.0f, 0.0f};
    return q;
}

void quat_normalize(quat_t *q) {
    float norm = sqrtf(q->w*q->w + q->x*q->x + q->y*q->y + q->z*q->z);
    if (norm > 0.0f) {
        q->w /= norm;
        q->x /= norm;
        q->y /= norm;
        q->z /= norm;
    }
}

quat_t quat_mul(quat_t q1, quat_t q2) {
    quat_t q;
    q.w = q1.w*q2.w - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z;
    q.x = q1.w*q2.x + q1.x*q2.w + q1.y*q2.z - q1.z*q2.y;
    q.y = q1.w*q2.y - q1.x*q2.z + q1.y*q2.w + q1.z*q2.x;
    q.z = q1.w*q2.z + q1.x*q2.y - q1.y*q2.x + q1.z*q2.w;
    return q;
}

vec3_t quat_rotate_vector(quat_t q, vec3_t v) {
    quat_t q_conj = {q.w, -q.x, -q.y, -q.z};
    quat_t v_quat = {0.0f, v.x, v.y, v.z};
    quat_t temp = quat_mul(q, v_quat);
    quat_t result = quat_mul(temp, q_conj);
    vec3_t rotated = {result.x, result.y, result.z};
    return rotated;
}

void quat_to_euler(quat_t q, float *roll, float *pitch, float *yaw) {
    *roll  = atan2f(2*(q.w*q.x + q.y*q.z), 1 - 2*(q.x*q.x + q.y*q.y));
    *pitch = asinf(2*(q.w*q.y - q.z*q.x));
    *yaw   = atan2f(2*(q.w*q.z + q.x*q.y), 1 - 2*(q.y*q.y + q.z*q.z));
}

quat_t quat_from_euler(float roll, float pitch, float yaw) {
    float cr = cosf(roll * 0.5f);
    float sr = sinf(roll * 0.5f);
    float cp = cosf(pitch * 0.5f);
    float sp = sinf(pitch * 0.5f);
    float cy = cosf(yaw * 0.5f);
    float sy = sinf(yaw * 0.5f);

    quat_t q;
    q.w = cr*cp*cy + sr*sp*sy;
    q.x = sr*cp*cy - cr*sp*sy;
    q.y = cr*sp*cy + sr*cp*sy;
    q.z = cr*cp*sy - sr*sp*cy;

    quat_normalize(&q);

    return q;
}
