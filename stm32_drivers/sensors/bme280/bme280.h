/*
 * bme280.h
 *
 *  Created on: Jan 25, 2026
 *      Author: H.Dani
 */

#ifndef INC_BME280_H_
#define INC_BME280_H_

#include "main.h"

#define BME_I2C_ADDR1	0x76
#define BME_I2C_ADDR2	0x77

#define I2C_TREAD_SIZE	0x03
#define I2C_PREAD_SIZE	0x03
#define I2C_HREAD_SIZE	0x02

#define REG_READ		0x80
#define REG_WRITE		0x00

#define SPI_TREAD_SIZE	0x04
#define SPI_PREAD_SIZE	0x04
#define SPI_HREAD_SIZE	0x03

#define TRIM_TSIZE		0x06
#define TRIM_PSIZE		0x12
#define TRIM_HSIZE		0x09

#define OVERSMPSKIP		0x00
#define OVERSMPX1  		0x01
#define OVERSMPX2  		0x02
#define OVERSMPX4  		0x03
#define OVERSMPX8  		0x04
#define OVERSMPX16 		0x05

#define REG_ID			0xD0
#define REG_RESET		0xE0
#define REG_CTRL_HUM	0xF2
#define REG_STATUS		0xF3
#define REG_CTRL_MEAS	0xF4
#define REG_CONFIG		0xF5
#define REG_PRESS_MSB	0xF7
#define REG_PRESS_LSB	0xF8
#define REG_PRESS_XLSB	0xF9
#define REG_TEMP_MSB	0xFA
#define REG_TEMP_LSB	0xFB
#define REG_TEMP_XLSB	0xFC
#define REG_HUM_MSB		0xFD
#define REG_HUM_LSB		0xFE

#define REG_DIGT1		0x88
#define REG_DIGT2		0x8A
#define REG_DIGT3		0x8C

#define REG_DIGP1		0x8E
#define REG_DIGP2		0x90
#define REG_DIGP3		0x92
#define REG_DIGP4		0x94
#define REG_DIGP5		0x96
#define REG_DIGP6		0x98
#define REG_DIGP7		0x9A
#define REG_DIGP8		0x9C
#define REG_DIGP9		0x9E

#define REG_DIGH1		0xA1
#define REG_DIGH2		0xE1
#define REG_DIGH3		0xE3
#define REG_DIGH4		0xE4
#define REG_DIGH5		0xE5
#define REG_DIGH6		0xE7

#define DIGT1_OFFSET	0x00
#define DIGT2_OFFSET	0x02
#define DIGT3_OFFSET	0x04

#define DIGP1_OFFSET	0x06
#define DIGP2_OFFSET	0x08
#define DIGP3_OFFSET	0x0A
#define DIGP4_OFFSET	0x0C
#define DIGP5_OFFSET	0x0E
#define DIGP6_OFFSET	0x10
#define DIGP7_OFFSET	0x12
#define DIGP8_OFFSET	0x14
#define DIGP9_OFFSET	0x16	//22
#define DIGH1_OFFSET	0x18

#define DIGH2_OFFSET  	0x00  // 0xE1, 0xE2
#define DIGH3_OFFSET  	0x02  // 0xE3
#define DIGH4_MSB     	0x03  // 0xE4
#define DIGH4_LSB     	0x04  // 0xE5 (low nibble)
#define DIGH5_LSB     	0x04  // 0xE5 (high nibble)
#define DIGH5_MSB     	0x05  // 0xE6
#define DIGH6_OFFSET  	0x06  // 0xE7

typedef struct{
	GPIO_TypeDef *gpio_port;
	uint16_t gpio_pin;
}GPIO_t;

#if defined(HAL_I2C_MODULE_ENABLED)
typedef struct{
	I2C_HandleTypeDef *i2c;
	GPIO_t addr_pin;
	uint8_t i2c_addr;
}I2C_com_t;
#endif

#if defined(HAL_SPI_MODULE_ENABLED)
typedef struct{
	SPI_HandleTypeDef *spi;
	GPIO_t cs_pin;
}SPI_com_t;
#endif

typedef union{
#if defined(HAL_I2C_MODULE_ENABLED)
	I2C_com_t bme_i2c;
#endif

#if defined(HAL_SPI_MODULE_ENABLED)
    SPI_com_t bme_spi;
#endif
}COM_protocol_t;

typedef struct{
	COM_protocol_t bme280_com;

	uint8_t temp_buff[3];
	uint8_t press_buff[3];
	uint8_t hum_buff[3];

	uint8_t calib_buff1[25];
	uint8_t calib_buff2[8];

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

BME280_t bme280_init(COM_protocol_t com_prot);

int32_t BME280_compensate_T_int32(BME280_t *bme280_dev, int32_t adc_T);

uint32_t BME280_compensate_P_int64(BME280_t *bme280_dev, int32_t adc_P);

uint32_t bme280_compensate_H_int32(BME280_t *bme280_dev, int32_t adc_H);

#if defined(HAL_I2C_MODULE_ENABLED)

void bme280_read_temp_i2c(BME280_t *bme280_dev);
void bme280_read_press_i2c(BME280_t *bme280_dev);
void bme280_read_hum_i2c(BME280_t *bme280_dev);

#endif

#if defined(HAL_SPI_MODULE_ENABLED)

void CS_LOW(BME280_t *bme280_dev);
void CS_HIGH(BME280_t *bme280_dev);

void bme280_read_temp_spi(BME280_t *bme280_dev);
void bme280_read_press_spi(BME280_t *bme280_dev);
void bme280_read_hum_spi(BME280_t *bme280_dev);

void bme280_read_calib_param_spi(BME280_t *bme280_dev, uint8_t param_reg, uint8_t *buffer, uint16_t param_size);

#endif

#endif /* INC_BME280_H_ */
