/*
 * AS5600.c
 *
 *  Created on: Jan 17, 2026
 *      Author: H.Dani
 */

#include "AS5600.h"
#include "stm32h7xx_hal.h"

AS5600_t as5600_init(I2C_HandleTypeDef *as5600_i2c, uint8_t i2c_addr)
{
	AS5600_t m_enc;

	m_enc.as5600_i2c = as5600_i2c;
	m_enc.i2c_addr = i2c_addr;

	return m_enc;
}

void as5600_config(AS5600_t *as5600)
{

}

int as5600_get_angle(AS5600_t *as5600)
{
	HAL_I2C_Mem_Read_DMA(as5600->as5600_i2c, (uint16_t)(as5600->i2c_addr << 1),
						 AS5600_RAW_ANGLE_REG, I2C_MEMADD_SIZE_8BIT, as5600->angle_buf, 2);
}

//void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
//{
//    if(hi2c == &hi2c1)
//    {
//        uint16_t angle = (angle_buf[0] << 8) | angle_buf[1];
//        // Notify the task that new data is ready
//        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
//        xSemaphoreGiveFromISR(angle_semaphore, &xHigherPriorityTaskWoken);
//        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
//    }
//}
