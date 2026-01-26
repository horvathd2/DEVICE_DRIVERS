/*
 * bme280.h
 *
 *  Created on: Jan 25, 2026
 *      Author: H.Dani
 */

#ifndef INC_BME280_H_
#define INC_BME280_H_

#include "main.h"

typedef union{
#if defined(HAL_I2C_MODULE_ENABLED)
    I2C_HandleTypeDef *i2c;
#endif

#if defined(HAL_SPI_MODULE_ENABLED)
    SPI_HandleTypeDef *spi;
#endif
}COM_protocol_t;

typedef struct{
	COM_protocol_t bme280_com;
	uint8_t temperature;
	uint8_t pressure;
	uint8_t humidity;
}BME280_t;

#endif /* INC_BME280_H_ */
