/*
 * mpu6050.h
 *
 *  Created on: Oct 27, 2025
 *      Author: hdani
 */

#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

//#include "stm32f4xx_hal.h"

#include "main.h"

#define MPU6050_ADDR			0x68

#define MPU6050_ACCEL_REG		0x3B
#define MPU6050_GYRO_REG		0x43

#define MPU6050_PWR_REG1		0x6B
#define MPU6050_PWR_REG2		0x6C

#define MPU6050_INT_PIN_CFG		0x37
#define MPU6050_INT_EN			0x38

#define ACCEL_READ_BYTES		0x6
#define GYRO_READ_BYTES			0x6

typedef struct{
	GPIO_TypeDef *gpio_port;
	uint32_t *gpio_pin;
}GPIO_mpu6050;

typedef struct{
	I2C_HandleTypeDef *mpu6050_i2c;
	GPIO_mpu6050 *mpu6050_it;
	int16_t accel_val[3];
	int16_t gyro_val[3];
	uint8_t accel_buf[6];
	uint8_t gyro_buf[6];
	uint8_t i2c_tx;
	uint8_t i2c_addr;
}MPU6050_t;

MPU6050_t mpu6050_init(I2C_HandleTypeDef *mpu6050_i2c, uint8_t i2c_addr, GPIO_mpu6050 *mpu6050_it);
void mpu6050_read(MPU6050_t *mpu6050_dev, uint16_t MemAddress, uint8_t *pData, uint16_t Size, int16_t *val_buff);
void mpu6050_readall(MPU6050_t *mpu6050_dev);

#endif /* INC_MPU6050_H_ */
