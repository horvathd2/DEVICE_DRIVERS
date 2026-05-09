/*
 * AS5600.c
 *
 *  Created on: Jan 17, 2026
 *      Author: H.Dani
 */

#include "as5600.h"
#include "main.h"
#include "string.h"

static as_err_t as5600_read(AS5600_t *self, uint8_t reg, uint8_t *data, uint16_t len)
{
	if(HAL_I2C_Mem_Read(self->as5600_i2c, (uint16_t)(self->i2c_addr << 1), reg,
						I2C_MEMADD_SIZE_8BIT, data, len, HAL_MAX_DELAY) != HAL_OK)
		return AS_FAIL;

	//HAL_I2C_Mem_Read_DMA(hi2c, DevAddress, MemAddress, MemAddSize, pData, Size);

	return AS_OK;
}

static as_err_t as5600_write(AS5600_t *self, uint8_t reg, const uint8_t *data, uint16_t len)
{
	if(HAL_I2C_Mem_Write(self->as5600_i2c, (uint16_t)(self->i2c_addr << 1), reg,
						 I2C_MEMADD_SIZE_8BIT, data, len, HAL_MAX_DELAY) != HAL_OK)
		return AS_FAIL;

	return AS_OK;
}

as_err_t as5600_init(AS5600_t *self, I2C_HandleTypeDef *as5600_i2c, uint8_t i2c_addr)
{
	if (self == NULL || as5600_i2c == NULL)
			return AS_FAIL;

	memset(self, 0, sizeof(AS5600_t));

	self->read = as5600_read;
	self->write = as5600_write;
	self->delay = NULL;
	self->as5600_i2c = as5600_i2c;
	self->i2c_addr = i2c_addr;
	self->dma_rx_done = 0;

	return AS_OK;
}

as_err_t as5600_read_angle(AS5600_t *self)
{
	if(self->read(self, AS5600_RAW_ANGLE_REG, self->angle_buf, ANGLE_READ_SIZE) != AS_OK)
		return AS_FAIL;

	self->raw_angle = (uint16_t)(self->angle_buf[0] << 8) | self->angle_buf[1];
	return AS_OK;
}

