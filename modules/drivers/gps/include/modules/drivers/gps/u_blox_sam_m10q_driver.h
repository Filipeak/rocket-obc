#ifndef _U_BLOX_SAM_M10Q_DRIVER_H
#define _U_BLOX_SAM_M10Q_DRIVER_H

#include "modules/drivers/hal/gpio_driver.h"
#include "modules/drivers/hal/gpio_utils.h"
#include <stdbool.h>
#include <stddef.h>

#define UBLOX_SAM_M10Q_SENTENCE_LENGTH 83

/**
 * @brief The configuration of the u-blox SAM-M10Q GPS module
 */
typedef struct ublox_sam_m10q_config
{
    gpio_communication_config_t gpioConfig; /** The GPIO communication configuration */
} ublox_sam_m10q_config_t;

/**
 * @brief The data of the u-blox SAM-M10Q GPS module
 */
typedef struct ublox_sam_m10q_data
{
    bool isValid;                                  /** Is current data valid */
    char sentence[UBLOX_SAM_M10Q_SENTENCE_LENGTH]; /** Sentence buffer */
    size_t currentIndex;                           /** Current index of buffer */
    bool isFinishedSentence;                       /** Is current sentence ready */
} ublox_sam_m10q_data_t;

/**
 * @brief Initializes the u-blox SAM-M10Q GPS module with SPI
 *
 * @param config The configuration of the u-blox SAM-M10Q GPS module
 * @param spi The SPI instance to use
 * @param miso The MISO pin to use
 * @param mosi The MOSI pin to use
 * @param cs The CS pin to use
 * @param sck The SCK pin to use
 */
void ublox_sam_m10q_init_spi(ublox_sam_m10q_config_t *config, spi_instance_t spi, pin_number_t miso, pin_number_t mosi, pin_number_t cs, pin_number_t sck);

/**
 * @brief Initializes the u-blox SAM-M10Q GPS module with I2C
 *
 * @param config The configuration of the u-blox SAM-M10Q GPS module
 * @param i2c The I2C instance to use
 * @param sda The SDA pin to use
 * @param scl The SCL pin to use
 */
void ublox_sam_m10q_init_i2c(ublox_sam_m10q_config_t *config, i2c_instance_t i2c, pin_number_t sda, pin_number_t scl);

/**
 * @brief Reads a NMEA sentence from the u-blox SAM-M10Q GPS module
 *
 * @param config The configuration of the u-blox SAM-M10Q GPS module
 * @param data The data of the u-blox SAM-M10Q GPS module
 */
void ublox_sam_m10q_read_data(ublox_sam_m10q_config_t *config, ublox_sam_m10q_data_t *data);

#endif