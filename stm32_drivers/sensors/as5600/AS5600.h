/*
 * AS5600.h
 *
 *  Created on: Jan 17, 2026
 *      Author: H.Dani
 */

#ifndef INC_AS5600_H_
#define INC_AS5600_H_

#include "stm32h7xx_hal.h"

#define AS5600_ADDR				0x36

#define AS5600_ZMCO_REG			0x00
#define AS5600_ZPOS_REG			0x01
#define AS5600_MPOS_REG			0x03
#define AS5600_MANG_REG			0x05
#define AS5600_CONF_REG			0x07

#define AS5600_RAW_ANGLE_REG 	0x0C 	//To 0x0D
#define AS5600_ANGLE_REG		0x0E	//To 0x0F

#define AS5600_STATUS_REG		0x0B
#define AS5600_AGC_REG			0x1A
#define AS5600_MAGNITUDE_REG	0x1B

#define AS5600_BURN				0xFF

typedef struct {
	I2C_HandleTypeDef *as5600_i2c;
	float filtered_angle;
	float raw_angle;
	float magnitude;
	uint8_t angle_buf[2];
	uint8_t i2c_addr;
}AS5600_t;

AS5600_t init_encoder(I2C_HandleTypeDef *as5600_i2c, uint8_t i2c_addr);

#endif /* INC_AS5600_H_ */
