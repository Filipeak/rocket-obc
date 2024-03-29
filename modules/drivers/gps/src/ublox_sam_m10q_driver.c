#include "modules/drivers/gps/ublox_sam_m10q_driver.h"
#include <string.h>

#define I2C_ADDRESS 0x42

void ublox_sam_m10q_init_spi(ublox_sam_m10q_config_t *config, hal_spi_instance_t spi, hal_pin_number_t cs)
{
    config->gpioConfig = (hal_gpio_communication_config_t){
        .protocol = GPIO_PROTOCOL_SPI,
        .spi = spi,
        .cs = cs,
    };

    hal_spi_init_cs(spi, cs);
}

void ublox_sam_m10q_init_i2c(ublox_sam_m10q_config_t *config, hal_i2c_instance_t i2c)
{
    config->gpioConfig = (hal_gpio_communication_config_t){
        .protocol = GPIO_PROTOCOL_I2C,
        .i2c = i2c,
        .i2cAddress = I2C_ADDRESS,
    };
}

void ublox_sam_m10q_read_data(ublox_sam_m10q_config_t *config, ublox_sam_m10q_data_t *data)
{
    uint8_t b = hal_gpio_single_read(&config->gpioConfig);

    if (b == 0xFF)
    {
        return;
    }

    if (b == '$')
    {
        data->currentIndex = 0;
        data->sentence.isFinished = false;

        memset(data->sentence.data, 0, sizeof(data->sentence.data));
    }
    else if (b == '\n')
    {
        data->sentence.data[data->currentIndex] = b;
        data->sentence.data[data->currentIndex + 1] = '\0';
        data->sentence.isFinished = true;

        return;
    }

    data->sentence.data[data->currentIndex++] = b;
}