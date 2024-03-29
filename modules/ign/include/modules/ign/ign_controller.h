#ifndef _IGN_CONTROLLER_H
#define _IGN_CONTROLLER_H

#include "modules/drivers/hal/gpio_driver.h"
#include "modules/drivers/hal/time_tracker.h"
#include "modules/drivers/adc/max1161X_driver.h"
#include "modules/flight_sm/flight_sm_control.h"
#include <stdbool.h>
#include <stddef.h>

/**
 * @brief Igniters pins config
 */
typedef struct ign_pins
{
    hal_pin_number_t main;
    hal_pin_number_t drouge;
    hal_pin_number_t sep;
    hal_pin_number_t second;
    max1161x_channel_t checkMain;
    max1161x_channel_t checkDrouge;
    max1161x_channel_t checkSep;
    max1161x_channel_t checkSecond;
} ign_pins_t;

/**
 * @brief Igniter settings
 */
typedef struct ign_settings
{
    float mainAlt;
    msec_t secondIgnDelay;
} ign_settings_t;

/**
 * @brief Data for firing single pin
 */
typedef struct ign_pin_data
{
    hal_pin_number_t pin;
    max1161x_channel_t checkPin;
    bool setHigh;
    bool fired;
    msec_t delay;
    msec_t time;
    bool finished;
} ign_pin_data_t;

/**
 * @brief Igniter data structure
 */
typedef struct ign_data
{
    bool armed;
    ign_settings_t settings;
    bool onlyMain;
    bool twoStages;
    ign_pin_data_t mainData;
    ign_pin_data_t drougeData;
    ign_pin_data_t sepData;
    ign_pin_data_t secondData;
} ign_data_t;

/**
 * @brief Igniter pin state data
 */
typedef struct ign_pin_state
{
    bool fuseWorking;
    bool ignPresent;
} ign_pin_state_t;

/**
 * @brief Initialize igniters
 *
 * @param data Data
 * @param pins Pins
 * @param Settings settings
 */
void ign_init(ign_data_t *data, const ign_pins_t *pins, const ign_settings_t *settings);

/**
 * @brief Arms igniters
 *
 * @param data Data
 */
void ign_arm(ign_data_t *data);

/**
 * @brief Update igniters
 *
 * @param data Data
 * @param sm Flight State Machine data pointer
 */
void ign_update(ign_data_t *data, const flight_sm_data_t *sm);

/**
 * @brief Check provided pin state
 *
 * @param adcConfig External ADC config
 * @param pinData Pointer to pin data
 * @param state Pointer to state to return
 */
void ign_check_pin(max1161x_config_t *adcConfig, const ign_pin_data_t *pinData, ign_pin_state_t *state);

#endif