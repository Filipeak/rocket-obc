#ifndef _MAX1161X_DRIVER_H
#define _MAX1161X_DRIVER_H

#include "modules/drivers/hal/i2c_driver.h"
#include <stdint.h>

/**
 * @brief MAX1161X configuration
 */
typedef struct max1161x_config
{
    hal_i2c_instance_t i2c; /** Selected I2C */
    uint8_t address;        /** I2C Address */
    float vRef;             /** Reference voltage */
} max1161x_config_t;

/**
 * @brief MAX1161X type
 */
typedef enum max1161x_type
{
    MAX1161X_TYPE_2,
    MAX1161X_TYPE_3,
    MAX1161X_TYPE_4,
    MAX1161X_TYPE_5,
    MAX1161X_TYPE_6,
    MAX1161X_TYPE_7,
} max1161x_type_t;

/**
 * @brief MAX1161X channel
 */
typedef enum max1161x_channel
{
    MAX1161X_CHANNEL_AIN0 = 0x00,
    MAX1161X_CHANNEL_AIN1 = 0x01,
    MAX1161X_CHANNEL_AIN2 = 0x02,
    MAX1161X_CHANNEL_AIN3 = 0x03,
    MAX1161X_CHANNEL_AIN4 = 0x04,
    MAX1161X_CHANNEL_AIN5 = 0x05,
    MAX1161X_CHANNEL_AIN6 = 0x06,
    MAX1161X_CHANNEL_AIN7 = 0x07,
    MAX1161X_CHANNEL_AIN8 = 0x08,
    MAX1161X_CHANNEL_AIN9 = 0x09,
    MAX1161X_CHANNEL_AIN10 = 0x0A,
    MAX1161X_CHANNEL_AIN11 = 0x0B,
} max1161x_channel_t;

/**
 * @brief Initializes MAX1161X
 *
 * @param config Configuration
 * @param type Type
 * @param i2c I2C instance
 */
void max1161x_init(max1161x_config_t *config, max1161x_type_t type, hal_i2c_instance_t i2c);

/**
 * @brief Reads MAX1161X value
 *
 * @param config Configuration
 * @param channel Channel
 * @param voltage Voltage value to return
 */
void max1161x_read(max1161x_config_t *config, max1161x_channel_t channel, hal_voltage_level_t *voltage);

#endif