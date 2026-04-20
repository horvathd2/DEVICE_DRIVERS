/*
 * bme280.c
 *
 *  Created on: Jan 25, 2026
 *      Author: H.Dani
 */

#include "bme280.h"
#include "main.h"

/* Trimming/calibration parameters should only be read once at initialization */
static bme_err_t bme280_get_calib_param(BME280_t *bme280_dev)
{
	if (bme280_dev == NULL || bme280_dev->read == NULL)
		return BME_FAIL;
	/* Read temperature/pressure/and hum1 calibration parameters */
	if(bme280_dev->read(bme280_dev, REG_DIGT1,
					   	bme280_dev->calib_buff1, TRIM_SIZE1) != BME_OK)
	{
		return BME_FAIL;
	}

	/* Read the rest of the humidity calibration parameters */
	if(bme280_dev->read(bme280_dev, REG_DIGH2,
					   	bme280_dev->calib_buff2, TRIM_SIZE2) != BME_OK)
	{
		return BME_FAIL;
	}

	bme280_dev->dig_T1 = (uint16_t)((bme280_dev->calib_buff1[DIGT1_OFFSET+1] << 8) | bme280_dev->calib_buff1[DIGT1_OFFSET]);
	bme280_dev->dig_T2 = (int16_t)((bme280_dev->calib_buff1[DIGT2_OFFSET+1] << 8) | bme280_dev->calib_buff1[DIGT2_OFFSET]);
	bme280_dev->dig_T3 = (int16_t)((bme280_dev->calib_buff1[DIGT3_OFFSET+1] << 8) | bme280_dev->calib_buff1[DIGT3_OFFSET]);

	bme280_dev->dig_P1 = (uint16_t)((bme280_dev->calib_buff1[DIGP1_OFFSET+1] << 8) | bme280_dev->calib_buff1[DIGP1_OFFSET]);
	bme280_dev->dig_P2 = (int16_t)((bme280_dev->calib_buff1[DIGP2_OFFSET+1] << 8) | bme280_dev->calib_buff1[DIGP2_OFFSET]);
	bme280_dev->dig_P3 = (int16_t)((bme280_dev->calib_buff1[DIGP3_OFFSET+1] << 8) | bme280_dev->calib_buff1[DIGP3_OFFSET]);
	bme280_dev->dig_P4 = (int16_t)((bme280_dev->calib_buff1[DIGP4_OFFSET+1] << 8) | bme280_dev->calib_buff1[DIGP4_OFFSET]);
	bme280_dev->dig_P5 = (int16_t)((bme280_dev->calib_buff1[DIGP5_OFFSET+1] << 8) | bme280_dev->calib_buff1[DIGP5_OFFSET]);
	bme280_dev->dig_P6 = (int16_t)((bme280_dev->calib_buff1[DIGP6_OFFSET+1] << 8) | bme280_dev->calib_buff1[DIGP6_OFFSET]);
	bme280_dev->dig_P7 = (int16_t)((bme280_dev->calib_buff1[DIGP7_OFFSET+1] << 8) | bme280_dev->calib_buff1[DIGP7_OFFSET]);
	bme280_dev->dig_P8 = (int16_t)((bme280_dev->calib_buff1[DIGP8_OFFSET+1] << 8) | bme280_dev->calib_buff1[DIGP8_OFFSET]);
	bme280_dev->dig_P9 = (int16_t)((bme280_dev->calib_buff1[DIGP9_OFFSET+1] << 8) | bme280_dev->calib_buff1[DIGP9_OFFSET]);

	bme280_dev->dig_H1 = (uint8_t)(bme280_dev->calib_buff1[DIGH1_OFFSET]);
	bme280_dev->dig_H2 = (int16_t)((bme280_dev->calib_buff2[DIGH2_OFFSET+1] << 8) | bme280_dev->calib_buff2[DIGH2_OFFSET]);
	bme280_dev->dig_H3 = (uint8_t)(bme280_dev->calib_buff2[DIGH3_OFFSET]);
	bme280_dev->dig_H4 = (int16_t)((bme280_dev->calib_buff2[DIGH4_MSB] << 4) | (bme280_dev->calib_buff2[DIGH4_LSB] & 0x0F));
	bme280_dev->dig_H5 = (int16_t)((bme280_dev->calib_buff2[DIGH5_MSB] << 4) | (bme280_dev->calib_buff2[DIGH5_LSB] >> 4));
	bme280_dev->dig_H6 = (int8_t)(bme280_dev->calib_buff2[DIGH6_OFFSET]);

	return BME_OK;
}

// Returns temperature in DegC, resolution is 0.01 DegC. Output value of “5123” equals 51.23 DegC.
// t_fine carries fine temperature as global value
static int32_t BME280_compensate_T_int32(BME280_t *bme280_dev, int32_t adc_T)
{
	int32_t var1, var2, T;

	var1 = ((((adc_T>>3) - ((int32_t)bme280_dev->dig_T1<<1))) *
			((int32_t)bme280_dev->dig_T2)) >> 11;
	var2 = (((((adc_T>>4) - ((int32_t)bme280_dev->dig_T1)) *
			((adc_T>>4) - ((int32_t)bme280_dev->dig_T1))) >> 12) *
			((int32_t)bme280_dev->dig_T3)) >> 14;

	bme280_dev->t_fine = var1 + var2;
	T = (bme280_dev->t_fine * 5 + 128) >> 8;

	return T;
}

// Returns pressure in Pa as unsigned 32 bit integer in Q24.8 format (24 integer bits and 8fractional bits).
// Output value of “24674867” represents 24674867/256 = 96386.2 Pa = 963.862 hPa
static uint32_t BME280_compensate_P_int64(BME280_t *bme280_dev, int32_t adc_P)
{
	int64_t var1, var2, p;

	var1 = ((int64_t)bme280_dev->t_fine) - 128000;
	var2 = var1 * var1 * (int64_t)bme280_dev->dig_P6;
	var2 = var2 + ((var1*(int64_t)bme280_dev->dig_P5)<<17);
	var2 = var2 + (((int64_t)bme280_dev->dig_P4)<<35);
	var1 = ((var1 * var1 * (int64_t)bme280_dev->dig_P3)>>8) +
			((var1 * (int64_t)bme280_dev->dig_P2)<<12);
	var1 = (((((int64_t)1)<<47)+var1)) * ((int64_t)bme280_dev->dig_P1)>>33;

	if (var1 == 0) return 0; // avoid exception caused by division by zero

	p = 1048576-adc_P;
	p = (((p<<31)-var2)*3125)/var1;
	var1 = (((int64_t)bme280_dev->dig_P9) * (p>>13) * (p>>13)) >> 25;
	var2 = (((int64_t)bme280_dev->dig_P8) * p) >> 19;
	p = ((p + var1 + var2) >> 8) + (((int64_t)bme280_dev->dig_P7)<<4);

	return (uint32_t)p;
}

// Returns humidity in %RH as unsigned 32 bit integer in Q22.10 format (22 integer and 10fractional bits).
// Output value of “47445” represents 47445/1024 = 46.333 %RH
static uint32_t bme280_compensate_H_int32(BME280_t *bme280_dev, int32_t adc_H)
{
	int32_t v_x1_u32r;

	v_x1_u32r = (bme280_dev->t_fine - ((int32_t)76800));
	v_x1_u32r = (((((adc_H << 14) - (((int32_t)bme280_dev->dig_H4) << 20) -
			(((int32_t)bme280_dev->dig_H5) *v_x1_u32r)) +
			((int32_t)16384)) >> 15) * (((((((v_x1_u32r *
			((int32_t)bme280_dev->dig_H6)) >> 10) *
			(((v_x1_u32r * ((int32_t)bme280_dev->dig_H3)) >> 11) +
			((int32_t)32768))) >> 10) + ((int32_t)2097152)) *
			((int32_t)bme280_dev->dig_H2) +8192) >> 14));
	v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) *
			(v_x1_u32r >> 15)) >> 7) * ((int32_t)bme280_dev->dig_H1)) >> 4));
	v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
	v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);

	return (uint32_t)(v_x1_u32r>>12);
}

/*-----------I2C--------------*/

#if defined(HAL_I2C_MODULE_ENABLED)

bme_err_t bme280_init_i2c(BME280_t *bme280_dev,
						  I2C_HandleTypeDef *i2c,
						  uint8_t i2c_addr)
{
	if (bme280_dev == NULL || i2c == NULL)
		return BME_FAIL;

	bme280_dev->read = bme280_read_i2c;
	bme280_dev->bme280_i2c.i2c = i2c;
	bme280_dev->bme280_i2c.i2c_addr = i2c_addr;

	//ADD CALIBRATION CHECK? MORE FLAGS?
	if(bme280_get_calib_param(bme280_dev) != BME_OK)
		return BME_FAIL;

	return BME_OK;
}

bme_err_t bme280_read_i2c(BME280_t *bme280_dev, uint8_t reg, uint8_t *data_buff, uint16_t len)
{
	uint16_t dev_addr = bme280_dev->bme280_i2c.i2c_addr << 1;
	if(HAL_I2C_Mem_Read(bme280_dev->bme280_i2c.i2c, dev_addr, reg, I2C_MEMADD_SIZE_8BIT,
						data_buff, len, HAL_MAX_DELAY) != HAL_OK)
	{
		return BME_FAIL;
	}

	return BME_OK;
}

bme_err_t bme280_read_temp_i2c(BME280_t *bme280_dev)
{
	if(bme280_dev->read(bme280_dev, REG_TEMP_MSB,
						bme280_dev->temp_buff, TREAD_SIZE) != HAL_OK)
	{
		return BME_FAIL;
	}

	return BME_OK;
}

bme_err_t bme280_read_press_i2c(BME280_t *bme280_dev)
{
	if(bme280_dev->read(bme280_dev, REG_PRESS_MSB,
						bme280_dev->press_buff, PREAD_SIZE) != HAL_OK)
	{
		return BME_FAIL;
	}

	return BME_OK;
}

bme_err_t bme280_read_hum_i2c(BME280_t *bme280_dev)
{
	if(bme280_dev->read(bme280_dev, REG_HUM_MSB,
						bme280_dev->hum_buff, HREAD_SIZE) != HAL_OK)
	{
		return BME_FAIL;
	}

	return BME_OK;
}

#endif

/*-----------SPI--------------*/

#if defined(HAL_SPI_MODULE_ENABLED)

void CS_LOW(BME280_t *bme280_dev)
{
	HAL_GPIO_WritePin(bme280_dev->bme280_spi.cs_pin.gpio_port,
					  bme280_dev->bme280_spi.cs_pin.gpio_pin,
					  GPIO_PIN_RESET);
}

void CS_HIGH(BME280_t *bme280_dev)
{
	HAL_GPIO_WritePin(bme280_dev->bme280_spi.cs_pin.gpio_port,
					  bme280_dev->bme280_spi.cs_pin.gpio_pin,
					  GPIO_PIN_SET);
}

bme_err_t bme280_init_spi(BME280_t *bme280_dev,
						  SPI_HandleTypeDef *spi,
						  GPIO_t cs_pin)
{
	if (bme280_dev == NULL || spi == NULL)
		return BME_FAIL;

	bme280_dev->read = bme280_read_spi;
	bme280_dev->bme280_spi.spi = spi;
	bme280_dev->bme280_spi.cs_pin = cs_pin;

	//ADD CALIBRATION CHECK? MORE FLAGS?
	if(bme280_get_calib_param(bme280_dev) != BME_OK)
		return BME_FAIL;

	return BME_OK;
}

bme_err_t bme280_read_spi(BME280_t *bme280_dev, uint8_t reg, uint8_t *data_buff, uint16_t len)
{
	/* Reading starts by sending a control byte
	* (full register address without bit 7)
	* and the read command (bit 7 = RW = ‘1’) */
	uint8_t addr = reg | SPI_REG_READ;

	CS_LOW(bme280_dev);

	// 1. send register address
	if (HAL_SPI_Transmit(bme280_dev->bme280_spi.spi,
						 &addr, 1, HAL_MAX_DELAY) != HAL_OK)
	{
		CS_HIGH(bme280_dev);
		return BME_FAIL;
	}

	// 2. receive data (dummy transmit)
	if (HAL_SPI_Receive(bme280_dev->bme280_spi.spi,
						data_buff, len, HAL_MAX_DELAY) != HAL_OK)
	{
		CS_HIGH(bme280_dev);
		return BME_FAIL;
	}

	CS_HIGH(bme280_dev);

	return BME_OK;
}

bme_err_t bme280_read_temp_spi(BME280_t *bme280_dev)
{
	if(bme280_dev->read(bme280_dev, REG_TEMP_MSB,
					    bme280_dev->temp_buff, TREAD_SIZE) != BME_OK)
	{
		return BME_FAIL;
	}

	return BME_OK;
}

bme_err_t bme280_read_press_spi(BME280_t *bme280_dev)
{
	if(bme280_dev->read(bme280_dev, REG_PRESS_MSB,
					    bme280_dev->press_buff, PREAD_SIZE) != BME_OK)
	{
		return BME_FAIL;
	}

	return BME_OK;
}

bme_err_t bme280_read_hum_spi(BME280_t *bme280_dev)
{
	if(bme280_dev->read(bme280_dev, REG_HUM_MSB,
					    bme280_dev->hum_buff, HREAD_SIZE) != BME_OK)
	{
		return BME_FAIL;
	}

	return BME_OK;
}

#endif


