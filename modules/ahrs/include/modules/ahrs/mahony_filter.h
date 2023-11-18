#ifndef _MAHONY_FILTER_H
#define _MAHONY_FILTER_H

/**
 * REF: https://github.com/PaulStoffregen/MahonyAHRS/blob/master/src/MahonyAHRS.cpp
 * REF: https://nitinjsanket.github.io/tutorials/attitudeest/mahony.html
 */

#include "modules/maths/quaternion.h"
#include "modules/maths/vector.h"

/**
 * @brief Mahony filter data
 */
typedef struct mahony_filter_data
{
    quat_t q;
    float Kp;
    float Ki;
    float integralFBx;
    float integralFBy;
    float integralFBz;
    float samplePeriod;
} mahony_filter_data_t;

/**
 * @brief Initialize Mahony filter. Remember that the device should be aligned with the earth frame (or device frame if you want to use it)
 *
 * @param data Mahony filter data
 * @param kp Proportional gain (Optimal value: 0.5)
 * @param ki Integral gain (Optimal value: 0.0)
 * @param samplePeriod Sample period
 */
void mahony_init(mahony_filter_data_t *data, float kp, float ki, float samplePeriod);

/**
 * @brief Update Mahony filter without using magnetometer
 *
 * @param data Mahony filter data
 * @param gyroVec Gyrosopce vector in radians per second
 * @param accVec Accelerometer vector in meters per second squared
 */
void mahony_update_imu(mahony_filter_data_t *data, vec3_t gyroVec, vec3_t accVec);

/**
 * @brief Update Mahony filter using magnetometer
 *
 * @param data Mahony filter data
 * @param gyroVec Gyrosopce vector in radians per second
 * @param accVec Accelerometer vector in meters per second squared
 * @param magVec Magnetometer vector in Gauss
 */
void mahony_update_marg(mahony_filter_data_t *data, vec3_t gyroVec, vec3_t accVec, vec3_t magVec);

/**
 * @brief Internal function for updating Mahony filter
 *
 * @param data Mahony filter data
 * @param gyroVec Gyrosopce vector in radians per second
 * @param halfex Half of the error in x axis
 * @param halfey Half of the error in y axis
 * @param halfez Half of the error in z axis
 */
void _mahony_update_base(mahony_filter_data_t *data, vec3_t gyroVec, float halfex, float halfey, float halfez);

#endif