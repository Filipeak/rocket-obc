#include "ahrs/kalman_filter.h"
#include <string.h>

static vec3 rotateVectorThroughQuaternion(vec3 v, quat q)
{
    return (vec3){
        .x = v.x * (1 - 2 * (q.y * q.y + q.z * q.z)) + v.y * 2 * (q.x * q.y - q.z * q.w) + v.z * 2 * (q.x * q.z + q.y * q.w),
        .y = v.x * 2 * (q.x * q.y + q.z * q.w) + v.y * (1 - 2 * (q.x * q.x + q.z * q.z)) + v.z * 2 * (q.y * q.z - q.x * q.w),
        .z = v.x * 2 * (q.x * q.z - q.y * q.w) + v.y * 2 * (q.y * q.z + q.x * q.w) + v.z * (1 - 2 * (q.x * q.x + q.y * q.y)),
    };
}

VOID kalmanFilterInit(KalmanFilterState *state, KalmanFilterConfig *config)
{
    state->config = *config;

    memset(&state->stateVector, 0, sizeof(state->stateVector));
    memset(&state->stateCovariance, 0, sizeof(state->stateCovariance));
    memset(&state->measurementVector, 0, sizeof(state->measurementVector));
    memset(&state->stateTransitionMatrix, 0, sizeof(state->stateTransitionMatrix));
    memset(&state->processNoiseCovariance, 0, sizeof(state->processNoiseCovariance));
    memset(&state->measurementNoiseCovariance, 0, sizeof(state->measurementNoiseCovariance));
    memset(&state->kalmanGain, 0, sizeof(state->kalmanGain));

    state->stateVector.q.w = 1.0f;

    for (SIZE i = 0; i < 10; i++)
    {
        state->stateCovariance[i][i] = 1000.0f;
    }

    state->dt2 = state->config.dt * state->config.dt;
    state->dt3 = state->dt2 * state->config.dt;
    state->dt4 = state->dt3 * state->config.dt;
    state->dt5 = state->dt4 * state->config.dt;

    KalmanFilterInputData tempData = {0};

    __kalmanFilterPredictState(state, &tempData);
    __kalmanFilterPredictCovariance(state);
}

VOID kalmanFilterUpdate(KalmanFilterState *state, KalmanFilterInputData *inputData, KalmanFilterOutputData *pOutputData)
{
    __kalmanFilterUpdateGain(state, inputData);
    __kalmanFilterUpdateState(state, inputData);
    __kalmanFilterUpdateCovariance(state);

    *pOutputData = state->stateVector;

    __kalmanFilterPredictState(state, inputData);
    __kalmanFilterPredictCovariance(state);
}

VOID __kalmanFilterPredictState(KalmanFilterState *state, KalmanFilterInputData *inputData)
{
    vec3 rotatedAcc = rotateVectorThroughQuaternion(inputData->acc, state->stateVector.q);

    state->stateVector.pos.x = state->stateVector.pos.x + state->stateVector.vel.x * state->config.dt + rotatedAcc.x * state->dt2 / 2.0f;
    state->stateVector.pos.y = state->stateVector.pos.y + state->stateVector.vel.y * state->config.dt + rotatedAcc.y * state->dt2 / 2.0f;
    state->stateVector.pos.z = state->stateVector.pos.z + state->stateVector.vel.z * state->config.dt + rotatedAcc.z * state->dt2 / 2.0f;
    state->stateVector.vel.x = state->stateVector.vel.x + rotatedAcc.x * state->config.dt;
    state->stateVector.vel.y = state->stateVector.vel.y + rotatedAcc.y * state->config.dt;
    state->stateVector.vel.z = state->stateVector.vel.z + rotatedAcc.z * state->config.dt;
    state->stateVector.q.w = state->stateVector.q.w - state->config.dt * inputData->gyro.x * state->stateVector.q.x / 2.0f - state->config.dt * inputData->gyro.y * state->stateVector.q.y / 2.0f - state->config.dt * inputData->gyro.z * state->stateVector.q.z / 2.0f;
    state->stateVector.q.x = state->stateVector.q.x + state->config.dt * inputData->gyro.x * state->stateVector.q.w / 2.0f + state->config.dt * inputData->gyro.y * state->stateVector.q.z / 2.0f - state->config.dt * inputData->gyro.z * state->stateVector.q.y / 2.0f;
    state->stateVector.q.y = state->stateVector.q.y - state->config.dt * inputData->gyro.x * state->stateVector.q.z / 2.0f + state->config.dt * inputData->gyro.y * state->stateVector.q.w / 2.0f + state->config.dt * inputData->gyro.z * state->stateVector.q.x / 2.0f;
    state->stateVector.q.z = state->stateVector.q.z + state->config.dt * inputData->gyro.x * state->stateVector.q.y / 2.0f - state->config.dt * inputData->gyro.y * state->stateVector.q.x / 2.0f + state->config.dt * inputData->gyro.z * state->stateVector.q.w / 2.0f;

    state->stateTransitionMatrix[0][0] = 1.0f;
    state->stateTransitionMatrix[0][3] = state->config.dt;
    state->stateTransitionMatrix[0][6] = state->dt2 / 2.0f * (2 * state->stateVector.q.y * inputData->acc.z - 2 * state->stateVector.q.z * inputData->acc.y);
    state->stateTransitionMatrix[0][7] = state->dt2 / 2.0f * (2 * state->stateVector.q.y * inputData->acc.y - 2 * state->stateVector.q.z * inputData->acc.z);
    state->stateTransitionMatrix[0][8] = state->dt2 / 2.0f * (2 * state->stateVector.q.w * inputData->acc.z + 2 * state->stateVector.q.x * inputData->acc.y - 4 * state->stateVector.q.y * inputData->acc.x);
    state->stateTransitionMatrix[0][9] = state->dt2 / 2.0f * (-2 * state->stateVector.q.w * inputData->acc.y + 2 * state->stateVector.q.x * inputData->acc.z - 4 * state->stateVector.q.z * inputData->acc.x);

    state->stateTransitionMatrix[1][1] = 1.0f;
    state->stateTransitionMatrix[1][4] = state->config.dt;
    state->stateTransitionMatrix[1][6] = state->dt2 / 2.0f * (-2 * state->stateVector.q.x * inputData->acc.z + 2 * state->stateVector.q.z * inputData->acc.x);
    state->stateTransitionMatrix[1][7] = state->dt2 / 2.0f * (-2 * state->stateVector.q.w * inputData->acc.z - 4 * state->stateVector.q.x * inputData->acc.y + 2 * state->stateVector.q.y * inputData->acc.x);
    state->stateTransitionMatrix[1][8] = state->dt2 / 2.0f * (2 * state->stateVector.q.x * inputData->acc.x + 2 * state->stateVector.q.z * inputData->acc.z);
    state->stateTransitionMatrix[1][9] = state->dt2 / 2.0f * (2 * state->stateVector.q.w * inputData->acc.x + 2 * state->stateVector.q.y * inputData->acc.z - 4 * state->stateVector.q.z * inputData->acc.y);

    state->stateTransitionMatrix[2][2] = 1.0f;
    state->stateTransitionMatrix[2][5] = state->config.dt;
    state->stateTransitionMatrix[2][6] = state->dt2 / 2.0f * (2 * state->stateVector.q.x * inputData->acc.y - 2 * state->stateVector.q.y * inputData->acc.x);
    state->stateTransitionMatrix[2][7] = state->dt2 / 2.0f * (2 * state->stateVector.q.w * inputData->acc.y - 4 * state->stateVector.q.x * inputData->acc.z + 2 * state->stateVector.q.z * inputData->acc.x);
    state->stateTransitionMatrix[2][8] = state->dt2 / 2.0f * (-2 * state->stateVector.q.w * inputData->acc.x - 4 * state->stateVector.q.y * inputData->acc.z + 2 * state->stateVector.q.z * inputData->acc.y);
    state->stateTransitionMatrix[2][9] = state->dt2 / 2.0f * (2 * state->stateVector.q.x * inputData->acc.x + 2 * state->stateVector.q.y * inputData->acc.y);

    state->stateTransitionMatrix[3][3] = 1.0f;
    state->stateTransitionMatrix[3][6] = state->config.dt * (2 * state->stateVector.q.y * inputData->acc.z - 2 * state->stateVector.q.z * inputData->acc.y);
    state->stateTransitionMatrix[3][7] = state->config.dt * (2 * state->stateVector.q.y * inputData->acc.y + 2 * state->stateVector.q.z * inputData->acc.z);
    state->stateTransitionMatrix[3][8] = state->config.dt * (2 * state->stateVector.q.w * inputData->acc.z + 2 * state->stateVector.q.x * inputData->acc.y - 4 * state->stateVector.q.y * inputData->acc.x);
    state->stateTransitionMatrix[3][9] = state->config.dt * (-2 * state->stateVector.q.w * inputData->acc.y + 2 * state->stateVector.q.x * inputData->acc.z - 4 * state->stateVector.q.z * inputData->acc.x);

    state->stateTransitionMatrix[4][4] = 1.0f;
    state->stateTransitionMatrix[4][6] = state->config.dt * (-2 * state->stateVector.q.x * inputData->acc.z + 2 * state->stateVector.q.z * inputData->acc.x);
    state->stateTransitionMatrix[4][7] = state->config.dt * (-2 * state->stateVector.q.w * inputData->acc.z - 4 * state->stateVector.q.x * inputData->acc.y + 2 * state->stateVector.q.y * inputData->acc.x);
    state->stateTransitionMatrix[4][8] = state->config.dt * (2 * state->stateVector.q.x * inputData->acc.x + 2 * state->stateVector.q.z * inputData->acc.z);
    state->stateTransitionMatrix[4][9] = state->config.dt * (2 * state->stateVector.q.w * inputData->acc.x + 2 * state->stateVector.q.y * inputData->acc.z - 4 * state->stateVector.q.z * inputData->acc.y);

    state->stateTransitionMatrix[5][5] = 1.0f;
    state->stateTransitionMatrix[5][6] = state->config.dt * (2 * state->stateVector.q.x * inputData->acc.y - 2 * state->stateVector.q.y * inputData->acc.x);
    state->stateTransitionMatrix[5][7] = state->config.dt * (2 * state->stateVector.q.w * inputData->acc.y - 4 * state->stateVector.q.x * inputData->acc.z + 2 * state->stateVector.q.z * inputData->acc.x);
    state->stateTransitionMatrix[5][8] = state->config.dt * (-2 * state->stateVector.q.w * inputData->acc.x - 4 * state->stateVector.q.y * inputData->acc.z + 2 * state->stateVector.q.z * inputData->acc.y);
    state->stateTransitionMatrix[5][9] = state->config.dt * (2 * state->stateVector.q.x * inputData->acc.x + 2 * state->stateVector.q.y * inputData->acc.y);

    state->stateTransitionMatrix[6][6] = 1.0f;
    state->stateTransitionMatrix[6][7] = -state->config.dt * inputData->gyro.x / 2.0f;
    state->stateTransitionMatrix[6][8] = -state->config.dt * inputData->gyro.y / 2.0f;
    state->stateTransitionMatrix[6][9] = -state->config.dt * inputData->gyro.z / 2.0f;

    state->stateTransitionMatrix[7][6] = state->config.dt * inputData->gyro.x / 2.0f;
    state->stateTransitionMatrix[7][7] = 1.0f;
    state->stateTransitionMatrix[7][8] = state->config.dt * inputData->gyro.z / 2.0f;
    state->stateTransitionMatrix[7][9] = -state->config.dt * inputData->gyro.y / 2.0f;

    state->stateTransitionMatrix[8][6] = state->config.dt * inputData->gyro.y / 2.0f;
    state->stateTransitionMatrix[8][7] = -state->config.dt * inputData->gyro.z / 2.0f;
    state->stateTransitionMatrix[8][8] = 1.0f;
    state->stateTransitionMatrix[8][9] = state->config.dt * inputData->gyro.x / 2.0f;

    state->stateTransitionMatrix[9][6] = state->config.dt * inputData->gyro.z / 2.0f;
    state->stateTransitionMatrix[9][7] = state->config.dt * inputData->gyro.y / 2.0f;
    state->stateTransitionMatrix[9][8] = -state->config.dt * inputData->gyro.x / 2.0f;
    state->stateTransitionMatrix[9][9] = 1.0f;
}

VOID __kalmanFilterPredictCovariance(KalmanFilterState *state)
{
}

VOID __kalmanFilterUpdateState(KalmanFilterState *state, KalmanFilterInputData *inputData)
{
}

VOID __kalmanFilterUpdateCovariance(KalmanFilterState *state)
{
}

VOID __kalmanFilterUpdateGain(KalmanFilterState *state, KalmanFilterInputData *inputData)
{
}