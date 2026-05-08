/*
 * bno055.h
 *
 *  Created on: May 8, 2026
 *      Author: H.Dani
 */

#ifndef INC_BNO055_H_
#define INC_BNO055_H_

#include "main.h"

/* ================================================
   BNO055 Register Address Definitions (Page 0)
   ================================================ */

/* Chip IDs */
#define BNO055_CHIP_ID_ADDR              0x00
#define BNO055_ACC_ID_ADDR               0x01
#define BNO055_MAG_ID_ADDR               0x02
#define BNO055_GYR_ID_ADDR               0x03

/* Software / Bootloader Revision */
#define BNO055_SW_REV_ID_LSB_ADDR        0x04
#define BNO055_SW_REV_ID_MSB_ADDR        0x05
#define BNO055_BL_REV_ID_ADDR            0x06

/* Page ID */
#define BNO055_PAGE_ID_ADDR              0x07

/* Accelerometer Data */
#define BNO055_ACC_DATA_X_LSB_ADDR       0x08
#define BNO055_ACC_DATA_X_MSB_ADDR       0x09
#define BNO055_ACC_DATA_Y_LSB_ADDR       0x0A
#define BNO055_ACC_DATA_Y_MSB_ADDR       0x0B
#define BNO055_ACC_DATA_Z_LSB_ADDR       0x0C
#define BNO055_ACC_DATA_Z_MSB_ADDR       0x0D

/* Magnetometer Data */
#define BNO055_MAG_DATA_X_LSB_ADDR       0x0E
#define BNO055_MAG_DATA_X_MSB_ADDR       0x0F
#define BNO055_MAG_DATA_Y_LSB_ADDR       0x10
#define BNO055_MAG_DATA_Y_MSB_ADDR       0x11
#define BNO055_MAG_DATA_Z_LSB_ADDR       0x12
#define BNO055_MAG_DATA_Z_MSB_ADDR       0x13

/* Gyroscope Data */
#define BNO055_GYR_DATA_X_LSB_ADDR       0x14
#define BNO055_GYR_DATA_X_MSB_ADDR       0x15
#define BNO055_GYR_DATA_Y_LSB_ADDR       0x16
#define BNO055_GYR_DATA_Y_MSB_ADDR       0x17
#define BNO055_GYR_DATA_Z_LSB_ADDR       0x18
#define BNO055_GYR_DATA_Z_MSB_ADDR       0x19

/* Euler Angles (Heading, Roll, Pitch) */
#define BNO055_EUL_HEADING_LSB_ADDR      0x1A
#define BNO055_EUL_HEADING_MSB_ADDR      0x1B
#define BNO055_EUL_ROLL_LSB_ADDR         0x1C
#define BNO055_EUL_ROLL_MSB_ADDR         0x1D
#define BNO055_EUL_PITCH_LSB_ADDR        0x1E
#define BNO055_EUL_PITCH_MSB_ADDR        0x1F

/* Quaternion Data */
#define BNO055_QUA_DATA_W_LSB_ADDR       0x20
#define BNO055_QUA_DATA_W_MSB_ADDR       0x21
#define BNO055_QUA_DATA_X_LSB_ADDR       0x22
#define BNO055_QUA_DATA_X_MSB_ADDR       0x23
#define BNO055_QUA_DATA_Y_LSB_ADDR       0x24
#define BNO055_QUA_DATA_Y_MSB_ADDR       0x25
#define BNO055_QUA_DATA_Z_LSB_ADDR       0x26
#define BNO055_QUA_DATA_Z_MSB_ADDR       0x27

/* Linear Acceleration */
#define BNO055_LIA_DATA_X_LSB_ADDR       0x28
#define BNO055_LIA_DATA_X_MSB_ADDR       0x29
#define BNO055_LIA_DATA_Y_LSB_ADDR       0x2A
#define BNO055_LIA_DATA_Y_MSB_ADDR       0x2B
#define BNO055_LIA_DATA_Z_LSB_ADDR       0x2C
#define BNO055_LIA_DATA_Z_MSB_ADDR       0x2D

/* Gravity Vector */
#define BNO055_GRV_DATA_X_LSB_ADDR       0x2E
#define BNO055_GRV_DATA_X_MSB_ADDR       0x2F
#define BNO055_GRV_DATA_Y_LSB_ADDR       0x30
#define BNO055_GRV_DATA_Y_MSB_ADDR       0x31
#define BNO055_GRV_DATA_Z_LSB_ADDR       0x32
#define BNO055_GRV_DATA_Z_MSB_ADDR       0x33

/* Temperature */
#define BNO055_TEMP_ADDR                 0x34

/* Status Registers */
#define BNO055_CALIB_STAT_ADDR           0x35
#define BNO055_ST_RESULT_ADDR            0x36
#define BNO055_INT_STA_ADDR              0x37
#define BNO055_SYS_CLK_STATUS_ADDR       0x38
#define BNO055_SYS_STATUS_ADDR           0x39
#define BNO055_SYS_ERR_ADDR              0x3A

/* Configuration */
#define BNO055_UNIT_SEL_ADDR             0x3B
#define BNO055_OPR_MODE_ADDR             0x3D
#define BNO055_PWR_MODE_ADDR             0x3E
#define BNO055_SYS_TRIGGER_ADDR          0x3F
#define BNO055_TEMP_SOURCE_ADDR          0x40

/* Axis Mapping & Remap */
#define BNO055_AXIS_MAP_CONFIG_ADDR      0x41
#define BNO055_AXIS_MAP_SIGN_ADDR        0x42

/* SIC Matrix (Sensor Input Correction) */
#define BNO055_SIC_MATRIX_0_LSB_ADDR     0x43
#define BNO055_SIC_MATRIX_0_MSB_ADDR     0x44
#define BNO055_SIC_MATRIX_1_LSB_ADDR     0x45
#define BNO055_SIC_MATRIX_1_MSB_ADDR     0x46
#define BNO055_SIC_MATRIX_2_LSB_ADDR     0x47
#define BNO055_SIC_MATRIX_2_MSB_ADDR     0x48
#define BNO055_SIC_MATRIX_3_LSB_ADDR     0x49
#define BNO055_SIC_MATRIX_3_MSB_ADDR     0x4A
#define BNO055_SIC_MATRIX_4_LSB_ADDR     0x4B
#define BNO055_SIC_MATRIX_4_MSB_ADDR     0x4C
#define BNO055_SIC_MATRIX_5_LSB_ADDR     0x4D
#define BNO055_SIC_MATRIX_5_MSB_ADDR     0x4E
#define BNO055_SIC_MATRIX_6_LSB_ADDR     0x4F
#define BNO055_SIC_MATRIX_6_MSB_ADDR     0x50
#define BNO055_SIC_MATRIX_7_LSB_ADDR     0x51
#define BNO055_SIC_MATRIX_7_MSB_ADDR     0x52
#define BNO055_SIC_MATRIX_8_LSB_ADDR     0x53
#define BNO055_SIC_MATRIX_8_MSB_ADDR     0x54

/* Offsets */
#define BNO055_ACC_OFFSET_X_LSB_ADDR     0x55
#define BNO055_ACC_OFFSET_X_MSB_ADDR     0x56
#define BNO055_ACC_OFFSET_Y_LSB_ADDR     0x57
#define BNO055_ACC_OFFSET_Y_MSB_ADDR     0x58
#define BNO055_ACC_OFFSET_Z_LSB_ADDR     0x59
#define BNO055_ACC_OFFSET_Z_MSB_ADDR     0x5A

#define BNO055_MAG_OFFSET_X_LSB_ADDR     0x5B
#define BNO055_MAG_OFFSET_X_MSB_ADDR     0x5C
#define BNO055_MAG_OFFSET_Y_LSB_ADDR     0x5D
#define BNO055_MAG_OFFSET_Y_MSB_ADDR     0x5E
#define BNO055_MAG_OFFSET_Z_LSB_ADDR     0x5F
#define BNO055_MAG_OFFSET_Z_MSB_ADDR     0x60

#define BNO055_GYR_OFFSET_X_LSB_ADDR     0x61
#define BNO055_GYR_OFFSET_X_MSB_ADDR     0x62
#define BNO055_GYR_OFFSET_Y_LSB_ADDR     0x63
#define BNO055_GYR_OFFSET_Y_MSB_ADDR     0x64
#define BNO055_GYR_OFFSET_Z_LSB_ADDR     0x65
#define BNO055_GYR_OFFSET_Z_MSB_ADDR     0x66

/* Radius */
#define BNO055_ACC_RADIUS_LSB_ADDR       0x67
#define BNO055_ACC_RADIUS_MSB_ADDR       0x68
#define BNO055_MAG_RADIUS_LSB_ADDR       0x69
#define BNO055_MAG_RADIUS_MSB_ADDR       0x6A

#define BNO_OK		0
#define BNO_FAIL	1

typedef uint8_t bno_err_t;

typedef struct BMO055_t BMO055_t;

struct BMO055_t{

};

#endif /* INC_BNO055_H_ */
