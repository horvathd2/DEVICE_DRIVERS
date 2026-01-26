/*
 * dht11.h
 *
 *  Created on: Oct 27, 2025
 *      Author: hdani
 */

#ifndef INC_DHT11_H_
#define INC_DHT11_H_

#include "main.h"

//#include "stm32f4xx_hal.h"

typedef struct{
	GPIO_TypeDef *gpio_port;
	uint32_t gpio_pin;
}GPIO_dht11;

typedef struct{
	GPIO_dht11 dht11_pin;
	TIM_HandleTypeDef *htim_usdelay;
	uint8_t hum_int;
	uint8_t hum_dec;
	uint8_t temp_int;
	uint8_t temp_dec;
	uint8_t checksum;
}DHT11_t;

void delay_us(TIM_HandleTypeDef *htim_delay, uint32_t us);
void set_pin_mode(DHT11_t *dht11_sensor, uint32_t mode, uint32_t pull, uint32_t speed);
DHT11_t dht11_init(GPIO_dht11 dht11_pin, TIM_HandleTypeDef *htim_usdelay);
//uint8_t dht11_response(DHT11_t *dht11_sensor);
uint8_t dht11_read_data(DHT11_t *dht11_sensor);

#endif /* INC_DHT11_H_ */
