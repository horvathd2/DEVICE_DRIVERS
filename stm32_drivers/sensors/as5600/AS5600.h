/*
 * AS5600.h
 *
 *  Created on: Jan 17, 2026
 *      Author: H.Dani
 */

#ifndef INC_AS5600_H_
#define INC_AS5600_H_

#include "main.h"

#define AS5600_ADDR				0x36

#define AS5600_ZMCO_REG			0x00
#define AS5600_ZPOS_REG			0x01
#define AS5600_MPOS_REG			0x03
#define AS5600_MANG_REG			0x05
#define AS5600_CONF_REG			0x07

#define AS5600_RAW_ANGLE_REG 	0x0C 	//To 0x0D
#define AS5600_ANGLE_REG		0x0E	//To 0x0F

#define ANGLE_READ_SIZE			0x2

#define AS5600_STATUS_REG		0x0B
#define AS5600_AGC_REG			0x1A
#define AS5600_MAGNITUDE_REG	0x1B

#define AS5600_BURN_REG			0xFF
#define AS5600_ANGLE_COM		0x80
#define AS5600_BURN_SETTING		0x40

typedef struct {
	I2C_HandleTypeDef *as5600_i2c;
	float filtered_angle;
	uint16_t raw_angle;
	float magnitude;
	uint8_t angle_buf[2];
	uint8_t i2c_addr;
}AS5600_t;

AS5600_t as5600_init(I2C_HandleTypeDef *as5600_i2c, uint8_t i2c_addr);
uint8_t as5600_read_angle(AS5600_t *as5600);

#endif /* INC_AS5600_H_ */
