/*
 * bmp280.c
 *
 *  Created on: May 8, 2026
 *      Author: H.Dani
 */

#include "bmp280.h"
#include "main.h"
#include "string.h"

/* Trimming/calibration parameters should only be read once at initialization */
static bmp_err_t bmp280_get_calib_param(BMP280_t *bmp280_dev)
{
	if (bmp280_dev == NULL || bmp280_dev->read == NULL)
		return BMP_FAIL;
	/* Read temperature/pressure/and hum1 calibration parameters */
	if(bmp280_dev->read(bmp280_dev, REG_DIGT1,
						bmp280_dev->calib_buff1, TRIM_SIZE1) != BMP_OK)
	{
		return BMP_FAIL;
	}

	bmp280_dev->dig_T1 = (uint16_t)((bmp280_dev->calib_buff1[DIGT1_OFFSET+1] << 8) | bmp280_dev->calib_buff1[DIGT1_OFFSET]);
	bmp280_dev->dig_T2 = (int16_t)((bmp280_dev->calib_buff1[DIGT2_OFFSET+1] << 8) | bmp280_dev->calib_buff1[DIGT2_OFFSET]);
	bmp280_dev->dig_T3 = (int16_t)((bmp280_dev->calib_buff1[DIGT3_OFFSET+1] << 8) | bmp280_dev->calib_buff1[DIGT3_OFFSET]);

	bmp280_dev->dig_P1 = (uint16_t)((bmp280_dev->calib_buff1[DIGP1_OFFSET+1] << 8) | bmp280_dev->calib_buff1[DIGP1_OFFSET]);
	bmp280_dev->dig_P2 = (int16_t)((bmp280_dev->calib_buff1[DIGP2_OFFSET+1] << 8) | bmp280_dev->calib_buff1[DIGP2_OFFSET]);
	bmp280_dev->dig_P3 = (int16_t)((bmp280_dev->calib_buff1[DIGP3_OFFSET+1] << 8) | bmp280_dev->calib_buff1[DIGP3_OFFSET]);
	bmp280_dev->dig_P4 = (int16_t)((bmp280_dev->calib_buff1[DIGP4_OFFSET+1] << 8) | bmp280_dev->calib_buff1[DIGP4_OFFSET]);
	bmp280_dev->dig_P5 = (int16_t)((bmp280_dev->calib_buff1[DIGP5_OFFSET+1] << 8) | bmp280_dev->calib_buff1[DIGP5_OFFSET]);
	bmp280_dev->dig_P6 = (int16_t)((bmp280_dev->calib_buff1[DIGP6_OFFSET+1] << 8) | bmp280_dev->calib_buff1[DIGP6_OFFSET]);
	bmp280_dev->dig_P7 = (int16_t)((bmp280_dev->calib_buff1[DIGP7_OFFSET+1] << 8) | bmp280_dev->calib_buff1[DIGP7_OFFSET]);
	bmp280_dev->dig_P8 = (int16_t)((bmp280_dev->calib_buff1[DIGP8_OFFSET+1] << 8) | bmp280_dev->calib_buff1[DIGP8_OFFSET]);
	bmp280_dev->dig_P9 = (int16_t)((bmp280_dev->calib_buff1[DIGP9_OFFSET+1] << 8) | bmp280_dev->calib_buff1[DIGP9_OFFSET]);

	return BMP_OK;
}

bmp_err_t bmp280_configure(BMP280_t *bmp280_dev)
{
	uint8_t config_buff[2] = {
			BMP280_CTRL_MEAS(BMP280_OSRS_X1,
							 BMP280_OSRS_X1,
							 BMP280_MODE_NORMAL),
			BMP280_CONFIG(BMP280_STBY_0_5_MS,
						  BMP280_FILTER_OFF)
	};


	if(bmp280_dev->write(bmp280_dev, REG_CTRL_MEAS, &config_buff[1], 1) != BMP_OK)
		return BMP_FAIL;

	if(bmp280_dev->write(bmp280_dev, REG_CONFIG, &config_buff[2], 1) != BMP_OK)
		return BMP_FAIL;

	return BMP_OK;
}

// Returns temperature in DegC, resolution is 0.01 DegC. Output value of “5123” equals 51.23 DegC.
// t_fine carries fine temperature as global value
static int32_t BMP280_compensate_T_int32(BMP280_t *bmp280_dev, int32_t adc_T)
{
	int32_t var1, var2, T;

	var1 = ((((adc_T>>3) - ((int32_t)bmp280_dev->dig_T1<<1))) *
			((int32_t)bmp280_dev->dig_T2)) >> 11;
	var2 = (((((adc_T>>4) - ((int32_t)bmp280_dev->dig_T1)) *
			((adc_T>>4) - ((int32_t)bmp280_dev->dig_T1))) >> 12) *
			((int32_t)bmp280_dev->dig_T3)) >> 14;

	bmp280_dev->t_fine = var1 + var2;
	T = (bmp280_dev->t_fine * 5 + 128) >> 8;

	return T;
}

// Returns pressure in Pa as unsigned 32 bit integer in Q24.8 format (24 integer bits and 8fractional bits).
// Output value of “24674867” represents 24674867/256 = 96386.2 Pa = 963.862 hPa
static uint32_t BMP280_compensate_P_int64(BMP280_t *bmp280_dev, int32_t adc_P)
{
	int64_t var1, var2, p;

	var1 = ((int64_t)bmp280_dev->t_fine) - 128000;
	var2 = var1 * var1 * (int64_t)bmp280_dev->dig_P6;
	var2 = var2 + ((var1*(int64_t)bmp280_dev->dig_P5)<<17);
	var2 = var2 + (((int64_t)bmp280_dev->dig_P4)<<35);
	var1 = ((var1 * var1 * (int64_t)bmp280_dev->dig_P3)>>8) +
			((var1 * (int64_t)bmp280_dev->dig_P2)<<12);
	var1 = (((((int64_t)1)<<47)+var1)) * ((int64_t)bmp280_dev->dig_P1)>>33;

	if (var1 == 0) return 0; // avoid exception caused by division by zero

	p = 1048576-adc_P;
	p = (((p<<31)-var2)*3125)/var1;
	var1 = (((int64_t)bmp280_dev->dig_P9) * (p>>13) * (p>>13)) >> 25;
	var2 = (((int64_t)bmp280_dev->dig_P8) * p) >> 19;
	p = ((p + var1 + var2) >> 8) + (((int64_t)bmp280_dev->dig_P7)<<4);

	return (uint32_t)p;
}

/*-----------I2C--------------*/

#if defined(HAL_I2C_MODULE_ENABLED)

static bmp_err_t bmp280_read_i2c(BMP280_t *bmp280_dev, uint8_t reg, uint8_t *data_buff, uint16_t len)
{
	uint16_t dev_addr = bmp280_dev->bmp280_i2c.i2c_addr << 1;
	if(HAL_I2C_Mem_Read(bmp280_dev->bmp280_i2c.i2c, dev_addr, reg, I2C_MEMADD_SIZE_8BIT,
						data_buff, len, HAL_MAX_DELAY) != HAL_OK)
	{
		return BMP_FAIL;
	}

	return BMP_OK;
}

bmp_err_t bmp280_init_i2c(BMP280_t *bmp280_dev,
						  I2C_HandleTypeDef *i2c,
						  uint8_t i2c_addr)
{
	if (bmp280_dev == NULL || i2c == NULL)
		return BMP_FAIL;

	memset(bmp280_dev, 0, sizeof(BMP280_t));

	bmp280_dev->read = bmp280_read_i2c;
	bmp280_dev->write = NULL;
	bmp280_dev->delay = NULL;
	bmp280_dev->bmp280_i2c.i2c = i2c;
	bmp280_dev->bmp280_i2c.i2c_addr = i2c_addr;

	//ADD CALIBRATION CHECK? MORE FLAGS?
	if(bme280_get_calib_param(bmp280_dev) != BMP_OK)
		return BMP_FAIL;

	return BMP_OK;
}

#endif

/*-----------SPI--------------*/

#if defined(HAL_SPI_MODULE_ENABLED)

static void CS_LOW(BMP280_t *bmp280_dev)
{
	HAL_GPIO_WritePin(bmp280_dev->bmp280_spi.cs_pin.gpio_port,
					  bmp280_dev->bmp280_spi.cs_pin.gpio_pin,
					  GPIO_PIN_RESET);
}

static void CS_HIGH(BMP280_t *bmp280_dev)
{
	HAL_GPIO_WritePin(bmp280_dev->bmp280_spi.cs_pin.gpio_port,
					  bmp280_dev->bmp280_spi.cs_pin.gpio_pin,
					  GPIO_PIN_SET);
}

static bmp_err_t bmp280_read_spi(BMP280_t *bmp280_dev, uint8_t reg, uint8_t *data_buff, uint16_t len)
{
	/* Reading starts by sending a control byte
	* (full register address without bit 7)
	* and the read command (bit 7 = RW = ‘1’) */
	uint8_t addr = reg | SPI_REG_READ;

	CS_LOW(bmp280_dev);

	// 1. send register address
	if (HAL_SPI_Transmit(bmp280_dev->bmp280_spi.spi,
						 &addr, 1, HAL_MAX_DELAY) != HAL_OK)
	{
		CS_HIGH(bmp280_dev);
		return BMP_FAIL;
	}

	// 2. receive data (dummy transmit)
	if (HAL_SPI_Receive(bmp280_dev->bmp280_spi.spi,
						data_buff, len, HAL_MAX_DELAY) != HAL_OK)
	{
		CS_HIGH(bmp280_dev);
		return BMP_FAIL;
	}

	CS_HIGH(bmp280_dev);

	return BMP_OK;
}

static bmp_err_t bmp280_write_spi(BMP280_t *bmp280_dev, uint8_t reg, const uint8_t *data_buff, uint16_t len)
{
	uint8_t addr = reg | SPI_REG_WRITE;

	CS_LOW(bmp280_dev);

	if (HAL_SPI_Transmit(bmp280_dev->bmp280_spi.spi,
						 &addr, 1, HAL_MAX_DELAY) != HAL_OK)
	{
		CS_HIGH(bmp280_dev);
		return BMP_FAIL;
	}

	if (HAL_SPI_Transmit(bmp280_dev->bmp280_spi.spi,
						 data_buff, len, HAL_MAX_DELAY) != HAL_OK)
	{
		CS_HIGH(bmp280_dev);
		return BMP_FAIL;
	}

	CS_HIGH(bmp280_dev);

	return BMP_OK;
}

bmp_err_t bmp280_init_spi(BMP280_t *bmp280_dev,
						  SPI_HandleTypeDef *spi,
						  GPIO_t cs_pin)
{
	if (bmp280_dev == NULL || spi == NULL)
		return BMP_FAIL;

	memset(bmp280_dev, 0, sizeof(BMP280_t));

	bmp280_dev->read = bmp280_read_spi;
	bmp280_dev->write = bmp280_write_spi;
	bmp280_dev->delay = NULL;
	bmp280_dev->bmp280_spi.spi = spi;
	bmp280_dev->bmp280_spi.cs_pin = cs_pin;

	if (bmp280_dev->read(bmp280_dev, REG_ID, &bmp280_dev->chip_id, 1) != BMP_OK)
		return BMP_FAIL;

	if(bme280_get_calib_param(bmp280_dev) != BMP_OK)
		return BMP_FAIL;

	//if(bme280_configure(bmp280_dev) != BMP_OK)
	//	return BMP_FAIL;

	return BMP_OK;
}

#endif

/*-------------GENERIC---------------*/

bmp_err_t bmp280_read_temp(BMP280_t *bmp280_dev)
{
	if(bmp280_dev->read(bmp280_dev, REG_TEMP_MSB,
					    bmp280_dev->temp_buff, TREAD_SIZE) != BMP_OK)
	{
		return BMP_FAIL;
	}

	bmp280_dev->raw_temperature = ((int32_t)bmp280_dev->temp_buff[0] << 12) |
								  ((int32_t)bmp280_dev->temp_buff[1] << 4)  |
								  ((int32_t)bmp280_dev->temp_buff[2] >> 4);

	bmp280_dev->comp_temperature = BMP280_compensate_T_int32(bmp280_dev, bmp280_dev->raw_temperature);

	return BMP_OK;
}

bmp_err_t bmp280_read_press(BMP280_t *bmp280_dev)
{
	if(bmp280_dev->read(bmp280_dev, REG_PRESS_MSB,
					    bmp280_dev->press_buff, PREAD_SIZE) != BMP_OK)
	{
		return BMP_FAIL;
	}

	bmp280_dev->raw_pressure = ((int32_t)bmp280_dev->press_buff[0] << 12) |
							   ((int32_t)bmp280_dev->press_buff[1] << 4)  |
							   ((int32_t)bmp280_dev->press_buff[2] >> 4);

	bmp280_dev->comp_pressure = BMP280_compensate_P_int64(bmp280_dev, bmp280_dev->raw_pressure);

	return BMP_OK;
}

