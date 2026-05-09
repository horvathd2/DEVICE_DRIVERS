/*
 * bme280.h
 *
 *  Created on: Jan 25, 2026
 *      Author: H.Dani
 */

#ifndef INC_BME280_H_
#define INC_BME280_H_

#include "main.h"

#define BME_CHIP_ID		0x60

#define BME_I2C_ADDR1	0x76
#define BME_I2C_ADDR2	0x77

#define SPI_REG_READ	0x80
#define SPI_REG_WRITE	0x00

#define TREAD_SIZE		0x03
#define PREAD_SIZE		0x03
#define HREAD_SIZE		0x02

#define TRIM_SIZE1		0x1A
#define TRIM_SIZE2		0x08

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

#define BME280_HUM_OSRS_SKIPPED   0x00
#define BME280_HUM_OSRS_X1        0x01
#define BME280_HUM_OSRS_X2        0x02
#define BME280_HUM_OSRS_X4        0x03
#define BME280_HUM_OSRS_X8        0x04
#define BME280_HUM_OSRS_X16       0x05

#define BME280_OSRS_SKIP   0x00
#define BME280_OSRS_X1     0x01
#define BME280_OSRS_X2     0x02
#define BME280_OSRS_X4     0x03
#define BME280_OSRS_X8     0x04
#define BME280_OSRS_X16    0x05

#define BME280_MODE_SLEEP   0x00
#define BME280_MODE_FORCED  0x01
#define BME280_MODE_NORMAL  0x03

#define BME280_CTRL_MEAS(temp_osrs, press_osrs, mode) \
						((temp_osrs << 5) | (press_osrs << 2) | (mode))

#define BME280_STBY_0_5_MS   0x00
#define BME280_STBY_62_5_MS  0x01
#define BME280_STBY_125_MS   0x02
#define BME280_STBY_250_MS   0x03
#define BME280_STBY_500_MS   0x04
#define BME280_STBY_1000_MS  0x05
#define BME280_STBY_10_MS    0x06
#define BME280_STBY_20_MS    0x07

#define BME280_FILTER_OFF   0x00
#define BME280_FILTER_2     0x01
#define BME280_FILTER_4     0x02
#define BME280_FILTER_8     0x03
#define BME280_FILTER_16    0x04

#define BME280_CONFIG(t_sb, filter) \
					 ((t_sb << 5) | (filter << 2))

#define BME280_SOFT_RESET_CMD	0xB6

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
#define DIGP9_OFFSET	0x16  //22
#define DIGH1_OFFSET	0x19

#define DIGH2_OFFSET  	0x00  // 0xE1, 0xE2
#define DIGH3_OFFSET  	0x02  // 0xE3
#define DIGH4_MSB     	0x03  // 0xE4
#define DIGH4_LSB     	0x04  // 0xE5 (low nibble)
#define DIGH5_LSB     	0x04  // 0xE5 (high nibble)
#define DIGH5_MSB     	0x05  // 0xE6
#define DIGH6_OFFSET  	0x06  // 0xE7

typedef enum{
	BME_OK = 0x00,
	BME_FAIL = 0x01,
}bmp_status_t;


typedef struct BME280_t BME280_t;

typedef uint8_t bme_err_t;

typedef struct{
	GPIO_TypeDef *gpio_port;
	uint16_t gpio_pin;
}GPIO_t;

#if defined(HAL_I2C_MODULE_ENABLED)
typedef struct{
	I2C_HandleTypeDef *i2c;
	uint8_t i2c_addr;
}I2C_com_t;
#endif

#if defined(HAL_SPI_MODULE_ENABLED)
typedef struct{
	SPI_HandleTypeDef *spi;
	GPIO_t cs_pin;
}SPI_com_t;
#endif

struct BME280_t{
	bme_err_t (*read)(BME280_t *bme280_dev, uint8_t reg, uint8_t *data, uint16_t len);
	bme_err_t (*write)(BME280_t *bme280_dev, uint8_t reg, const uint8_t *data, uint16_t len);
	bme_err_t (*delay)(uint16_t ticks);

#if defined(HAL_I2C_MODULE_ENABLED)
	I2C_com_t bme280_i2c;
#endif

#if defined(HAL_SPI_MODULE_ENABLED)
	SPI_com_t bme280_spi;
#endif

	uint8_t temp_buff[3];
	uint8_t press_buff[3];
	uint8_t hum_buff[3];

	uint8_t calib_buff1[26];
	uint8_t calib_buff2[8];

	int32_t raw_temperature;
	int32_t raw_pressure;
	int32_t raw_humidity;

	int32_t comp_temperature;
	uint32_t comp_pressure;
	uint32_t comp_humidity;

	uint8_t chip_id;

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
};



#if defined(HAL_I2C_MODULE_ENABLED)

bme_err_t bme280_init_i2c(BME280_t *bme280_dev,I2C_HandleTypeDef *i2c, uint8_t i2c_addr);

#endif

#if defined(HAL_SPI_MODULE_ENABLED)

bme_err_t bme280_init_spi(BME280_t *bme280_dev, SPI_HandleTypeDef *spi, GPIO_t cs_pin);

#endif

bme_err_t bme280_read_temp(BME280_t *bme280_dev);
bme_err_t bme280_read_press(BME280_t *bme280_dev);
bme_err_t bme280_read_hum(BME280_t *bme280_dev);

#endif /* INC_BME280_H_ */
