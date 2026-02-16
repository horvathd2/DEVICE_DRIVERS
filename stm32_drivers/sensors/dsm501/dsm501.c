/*
 * dsm501.c
 *
 *  Created on: Feb 16, 2026
 *      Author: H.Dani
 */
#include "dsm501.h"
#include "main.h"

dsm501_t dsm501_init(TIM_HandleTypeDef *pwm_in1,
					 uint32_t pwm_in1_channel1,
					 uint32_t pwm_in1_channel2,
					 TIM_HandleTypeDef *pwm_in2,
					 uint32_t pwm_in2_channel1,
					 uint32_t pwm_in2_channel2)
{
	dsm501_t self;

	self.pwm_in1 = pwm_in1;
	self.pwm_in1_channel1 = pwm_in1_channel1;
	self.pwm_in1_channel2 = pwm_in1_channel2;

	self.pwm_in1 = pwm_in2;
	self.pwm_in1_channel1 = pwm_in2_channel1;
	self.pwm_in1_channel2 = pwm_in2_channel2;

	return self;
}

uint8_t dsm501_read1(dsm501_t dsm501_sensor)
{
	if(dsm501_sensor.pwm_in1->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	{
		uint32_t capture1 = HAL_TIM_ReadCapturedValue(dsm501_sensor.pwm_in1, TIM_CHANNEL_1);
		if(capture1)
		{
			uint32_t frequency = SystemCoreClock/capture1;
			uint32_t capture2 = HAL_TIM_ReadCapturedValue(dsm501_sensor.pwm_in1, TIM_CHANNEL_2);
			dsm501_sensor.duty_cycle1 = (10000 * capture2)/capture1;
		}
	}

	return DSM_OK;
}

uint8_t dsm501_read2(dsm501_t dsm501_sensor)
{
	if(dsm501_sensor.pwm_in2->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	{

	}

	return DSM_OK;
}
