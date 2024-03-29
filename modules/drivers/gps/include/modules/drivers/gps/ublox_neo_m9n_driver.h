#ifndef _U_BLOX_NEO_M9N_DRIVER_H
#define _U_BLOX_NEO_M9N_DRIVER_H

#include "gps_defines.h"
#include "modules/drivers/utils/gpio_utils.h"
#include <stddef.h>

/**
 * @brief The configuration of the u-blox NEO-M9N GPS module.
 */
typedef struct ublox_neo_m9n_config
{
    hal_gpio_communication_config_t gpioConfig; /** The GPIO communication configuration */
} ublox_neo_m9n_config_t;

/**
 * @brief The data of the u-blox NEO-M9N GPS module.
 */
typedef struct ublox_neo_m9n_data
{
    gps_nmea_sentence_t sentence; /** Sentence */
    size_t currentIndex;          /** Current index of buffer */
} ublox_neo_m9n_data_t;

/**
 * @brief Initializes the u-blox NEO-M9N GPS module.
 *
 * @param config The configuration of the u-blox NEO-M9N GPS module.
 * @param spi The SPI instance to use.
 * @param cs The CS pin to use.
 */
void ublox_neo_m9n_init_spi(ublox_neo_m9n_config_t *config, hal_spi_instance_t spi, hal_pin_number_t cs);

/**
 * @brief Reads a NMEA sentence from the u-blox NEO-M9N GPS module.
 *
 * @param config The configuration of the u-blox NEO-M9N GPS module.
 * @param data The data of the u-blox NEO-M9N GPS module.
 */
void ublox_neo_m9n_read_data(ublox_neo_m9n_config_t *config, ublox_neo_m9n_data_t *data);

#endif