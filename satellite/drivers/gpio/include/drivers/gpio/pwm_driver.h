#pragma once

#include "gpio_driver.h"
#include "tools/typedefs.h"

/**
 * @brief Initialize PWM.
 *
 * @param pin Pin to intialize.
 * @return Result code.
 */
FUNCRESULT pwmInit(PinNumber pin);