/*
 * dsm501.h
 *
 *  Created on: Feb 16, 2026
 *      Author: H.Dani
 */

#ifndef INC_DSM501_H_
#define INC_DSM501_H_

#include "main.h"

typedef enum{
	DSM_OK,
	DSM_FAULT,
}dsm501_status;

typedef struct{
	TIM_HandleTypeDef *pwm_in1;
	uint32_t pwm_in1_channel1;
	uint32_t pwm_in1_channel2;
	TIM_HandleTypeDef *pwm_in2;
	uint32_t pwm_in2_channel1;
	uint32_t pwm_in2_channel2;
	int16_t duty_cycle1;
	int16_t duty_cycle2;
}dsm501_t;

dsm501_t dsm501_init(TIM_HandleTypeDef *pwm_in1,
					 uint32_t pwm_in1_channel1,
					 uint32_t pwm_in1_channel2,
					 TIM_HandleTypeDef *pwm_in2,
					 uint32_t pwm_in2_channel1,
					 uint32_t pwm_in2_channel2);



#endif /* INC_DSM501_H_ */
