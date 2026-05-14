/*
 * bno055.c
 *
 *  Created on: May 8, 2026
 *      Author: H.Dani
 */

#include "bno055.h"
#include "main.h"
#include "string.h"

static bno_err_t bno055_reset(BNO055_t *self)
{
	uint8_t reset = 0x20;

	if(self->write(self, BNO055_SYS_TRIGGER_ADDR, &reset, 1) != BNO_OK)
		return BNO_FAIL;

	self->delay(500);

	return BNO_OK;
}

#if BNO055_USE_I2C && defined(HAL_I2C_MODULE_ENABLED)

static bno_err_t bno055_read_i2c(BNO055_t *self, uint8_t reg, uint8_t *data, uint16_t len)
{
//	uint8_t dev_addr = self->bno_i2c.i2c_addr << 1;
//
//	if(HAL_I2C_Mem_Read(self->bno_i2c.hi2c, dev_addr, reg, I2C_MEMADD_SIZE_8BIT,
//						data, len, HAL_MAX_DELAY) != HAL_OK)
//		return BNO_FAIL;
//
//	return BNO_OK;
	uint8_t dev_addr = self->bno_i2c.i2c_addr << 1;
	HAL_StatusTypeDef hal_ret;

	// Step 1: Send register address
	hal_ret = HAL_I2C_Master_Transmit(self->bno_i2c.hi2c, dev_addr, &reg, 1, 100);
	if (hal_ret != HAL_OK)
		return BNO_FAIL;

	// Step 2: Read data with Repeated Start
	hal_ret = HAL_I2C_Master_Receive(self->bno_i2c.hi2c, dev_addr | 0x01, data, len, 200);
	if (hal_ret != HAL_OK)
		return BNO_FAIL;

	return BNO_OK;
}

static bno_err_t bno055_write_i2c(BNO055_t *self, uint8_t reg, const uint8_t *data, uint16_t len)
{
	uint8_t dev_addr = self->bno_i2c.i2c_addr << 1;

	if(HAL_I2C_Mem_Write(self->bno_i2c.hi2c, dev_addr, reg, I2C_MEMADD_SIZE_8BIT,
						 (uint8_t*)data, len, HAL_MAX_DELAY) != HAL_OK)
		return BNO_FAIL;

	return BNO_OK;
}

bno_err_t bno055_init_i2c(BNO055_t *self,
						  I2C_HandleTypeDef *hi2c,
						  uint8_t i2c_addr)
{
	if(self == NULL || hi2c == NULL)
		return BNO_FAIL;

	memset(self, 0, sizeof(BNO055_t));

	self->bno_i2c.hi2c = hi2c;
	self->bno_i2c.i2c_addr = i2c_addr;
	self->read = bno055_read_i2c;
	self->write = bno055_write_i2c;
	self->delay = HAL_Delay;

	self->delay(650);

	//if(bno055_reset(self) != BNO_OK)
	//	return BNO_FAIL;

	return BNO_OK;
}

#endif

#if BNO055_USE_UART && defined(HAL_UART_MODULE_ENABLED)



bno_err_t bno055_init_uart(BNO055_t *self
						   UART_HandleTypeDef *huart)
{
	if(self == NULL || huart == NULL)
		return BNO_FAIL;

	memset(self, 0, sizeof(BNO055_t));

	self->bno_uart.huart = huart;
	self->read = NULL;
	self->write = NULL;
	self->delay = HAL_Delay;

	return BNO_OK;
}

#endif


