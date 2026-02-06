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

	int32_t raw_temperature;
	uint32_t raw_pressure;
	uint32_t raw_humidity;

	int32_t comp_temperature;
	uint32_t comp_pressure;
	uint32_t comp_humidity;

	int32_t t_fine;

	uint16_t dig_T1;	//0x88
	int16_t	dig_T2;		//0x8A
	int16_t	dig_T3;		//0x8C

	uint16_t dig_P1;	//0x8E
	int16_t dig_P2;		//0x90
	int16_t dig_P3;		//0x92
	int16_t dig_P4;		//0x94
	int16_t dig_P5;		//0x96
	int16_t dig_P6;		//0x98
	int16_t dig_P7;		//0x9A
	int16_t dig_P8;		//0x9C
	int16_t dig_P9;		//0x9E

	uint8_t dig_H1;		//0xA1
	int16_t dig_H2;		//0xE1
	uint8_t dig_H3;		//0xE3
	int16_t dig_H4;		//0xE4
	int16_t dig_H5;		//0xE5
	int8_t dig_H6;		//0xE7
}BME280_t;



#endif /* INC_BME280_H_ */
