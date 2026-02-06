/*
 * bme280.c
 *
 *  Created on: Jan 25, 2026
 *      Author: H.Dani
 */

#include "bme280.h"
#include "main.h"

BME280_t bme280_init(COM_protocol_t com_prot)
{
	BME280_t bme280_dev = {0};

	bme280_dev.bme280_com = com_prot;

	return bme280_dev;
}

// Returns temperature in DegC, resolution is 0.01 DegC. Output value of “5123” equals 51.23 DegC.
// t_fine carries fine temperature as global value
int32_t BME280_compensate_T_int32(BME280_t *bme280_dev, int32_t adc_T)
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
uint32_t BME280_compensate_P_int64(BME280_t *bme280_dev, int32_t adc_P)
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
uint32_t bme280_compensate_H_int32(BME280_t *bme280_dev, int32_t adc_H)
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

#if defined(HAL_I2C_MODULE_ENABLED)
void bme280_read_i2c(BME280_t *bme280_dev)
{
	HAL_SPI_Receive(bme280_dev->bme280_com.spi, bme280_dev->temperature, 2, HAL_MAX_DELAY);
}
#endif

#if defined(HAL_SPI_MODULE_ENABLED)
void bme280_read_spi(BME280_t *bme280_dev)
{
	HAL_SPI_Receive(bme280_dev->bme280_com.spi, bme280_dev->temperature, 2, HAL_MAX_DELAY);
}
#endif


