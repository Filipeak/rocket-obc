#pragma once

#include "measurements_utils.h"

#define MEASUREMENTS_RADIO_COMMAND_ID 'M'

void initializeRadio();
void sendRadio(measurement_data_t *data);