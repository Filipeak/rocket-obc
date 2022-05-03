#pragma once

#include <stdio.h>

//#define PICO_DEFAULT_I2C BMP280_I2C
//#define PICO_DEFAULT_I2C_SDA_PIN BMP280_I2C_SDA_PIN
//#define PICO_DEFAULT_I2C_SCL_PIN BMP280_I2C_SCL_PIN

#define BMP280_ADDR _u(0x76)

#define BMP280_REG_CONFIG _u(0xF5)
#define BMP280_REG_CTRL_MEAS _u(0xF4)
#define BMP280_REG_RESET _u(0xE0)

#define BMP280_REG_TEMP_XLSB _u(0xFC)
#define BMP280_REG_TEMP_LSB _u(0xFB)
#define BMP280_REG_TEMP_MSB _u(0xFA)

#define BMP280_REG_PRESSURE_XLSB _u(0xF9)
#define BMP280_REG_PRESSURE_LSB _u(0xF8)
#define BMP280_REG_PRESSURE_MSB _u(0xF7)

#define BMP280_REG_DIG_T1_LSB _u(0x88)
#define BMP280_REG_DIG_T1_MSB _u(0x89)
#define BMP280_REG_DIG_T2_LSB _u(0x8A)
#define BMP280_REG_DIG_T2_MSB _u(0x8B)
#define BMP280_REG_DIG_T3_LSB _u(0x8C)
#define BMP280_REG_DIG_T3_MSB _u(0x8D)
#define BMP280_REG_DIG_P1_LSB _u(0x8E)
#define BMP280_REG_DIG_P1_MSB _u(0x8F)
#define BMP280_REG_DIG_P2_LSB _u(0x90)
#define BMP280_REG_DIG_P2_MSB _u(0x91)
#define BMP280_REG_DIG_P3_LSB _u(0x92)
#define BMP280_REG_DIG_P3_MSB _u(0x93)
#define BMP280_REG_DIG_P4_LSB _u(0x94)
#define BMP280_REG_DIG_P4_MSB _u(0x95)
#define BMP280_REG_DIG_P5_LSB _u(0x96)
#define BMP280_REG_DIG_P5_MSB _u(0x97)
#define BMP280_REG_DIG_P6_LSB _u(0x98)
#define BMP280_REG_DIG_P6_MSB _u(0x99)
#define BMP280_REG_DIG_P7_LSB _u(0x9A)
#define BMP280_REG_DIG_P7_MSB _u(0x9B)
#define BMP280_REG_DIG_P8_LSB _u(0x9C)
#define BMP280_REG_DIG_P8_MSB _u(0x9D)
#define BMP280_REG_DIG_P9_LSB _u(0x9E)
#define BMP280_REG_DIG_P9_MSB _u(0x9F)

#define BMP280_NUM_CALIB_PARAMS 24

struct bmp280_calib_param
{
    uint16_t dig_t1;
    int16_t dig_t2;
    int16_t dig_t3;

    uint16_t dig_p1;
    int16_t dig_p2;
    int16_t dig_p3;
    int16_t dig_p4;
    int16_t dig_p5;
    int16_t dig_p6;
    int16_t dig_p7;
    int16_t dig_p8;
    int16_t dig_p9;
} params;

typedef struct bmp280_data
{
    int32_t temperature;
    int32_t pressure;
} bmp280_data_t;

void bmp280Init();
void bmp280Read(bmp280_data_t* data);