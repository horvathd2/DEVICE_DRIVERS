/*
 * mpu6050.c
 *
 *  Created on: Oct 27, 2025
 *      Author: hdani
 */
#include "mpu6050.h"
#include "main.h"

//#include "stm32f4xx_hal.h"

MPU6050_t mpu6050_init(I2C_HandleTypeDef *mpu6050_i2c, uint8_t i2c_addr, GPIO_mpu6050 *mpu6050_it)
{
	MPU6050_t mpu6050_dev;

	mpu6050_dev.mpu6050_i2c = mpu6050_i2c;
	mpu6050_dev.mpu6050_it = mpu6050_it;
	mpu6050_dev.i2c_addr = i2c_addr;

	mpu6050_dev.i2c_tx = 0;

	HAL_I2C_Mem_Write(mpu6050_dev.mpu6050_i2c, mpu6050_dev.i2c_addr << 1, MPU6050_PWR_REG1,
					I2C_MEMADD_SIZE_8BIT, &mpu6050_dev.i2c_tx, 1, HAL_MAX_DELAY);

	return mpu6050_dev;
}

void mpu6050_read(MPU6050_t *mpu6050_dev, uint16_t MemAddress, uint8_t *pData, uint16_t Size, int16_t *val_buff)
{
	HAL_I2C_Mem_Read(mpu6050_dev->mpu6050_i2c, mpu6050_dev->i2c_addr << 1, MemAddress,
					I2C_MEMADD_SIZE_8BIT, pData, Size, HAL_MAX_DELAY);

	val_buff[0] = (int16_t)(pData[0] << 8 | pData[1]);
	val_buff[1] = (int16_t)(pData[2] << 8 | pData[3]);
	val_buff[2] = (int16_t)(pData[4] << 8 | pData[5]);
}

void mpu6050_readall(MPU6050_t *mpu6050_dev)
{
	HAL_I2C_Mem_Read(mpu6050_dev->mpu6050_i2c, mpu6050_dev->i2c_addr << 1, MPU6050_ACCEL_REG,
					I2C_MEMADD_SIZE_8BIT, mpu6050_dev->accel_buf, ACCEL_READ_BYTES, HAL_MAX_DELAY);

	mpu6050_dev->accel_val[0] = (int16_t)(mpu6050_dev->accel_buf[0] << 8 | mpu6050_dev->accel_buf[1]);
	mpu6050_dev->accel_val[1] = (int16_t)(mpu6050_dev->accel_buf[2] << 8 | mpu6050_dev->accel_buf[3]);
	mpu6050_dev->accel_val[2] = (int16_t)(mpu6050_dev->accel_buf[4] << 8 | mpu6050_dev->accel_buf[5]);

	HAL_I2C_Mem_Read(mpu6050_dev->mpu6050_i2c, mpu6050_dev->i2c_addr << 1, MPU6050_GYRO_REG,
					I2C_MEMADD_SIZE_8BIT, mpu6050_dev->gyro_buf, GYRO_READ_BYTES, HAL_MAX_DELAY);

	mpu6050_dev->gyro_val[0] = (int16_t)(mpu6050_dev->gyro_buf[0] << 8 | mpu6050_dev->gyro_buf[1]);
	mpu6050_dev->gyro_val[1] = (int16_t)(mpu6050_dev->gyro_buf[2] << 8 | mpu6050_dev->gyro_buf[3]);
	mpu6050_dev->gyro_val[2] = (int16_t)(mpu6050_dev->gyro_buf[4] << 8 | mpu6050_dev->gyro_buf[5]);
}
