/*
 * dht11.c
 *
 *  Created on: Oct 27, 2025
 *      Author: hdani
 */
#include "dht11.h"
#include "main.h"

//#include "stm32f4xx_hal.h"

void delay_us(TIM_HandleTypeDef *htim_delay, uint32_t us)
{
	__HAL_TIM_SET_COUNTER(htim_delay,0);  // set the counter value a 0
	while (__HAL_TIM_GET_COUNTER(htim_delay) < us);
}

void set_pin_mode(DHT11_t *dht11_sensor, uint32_t mode, uint32_t speed)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = dht11_sensor->dht11_pin->gpio_pin;
	GPIO_InitStruct.Mode = mode; //GPIO_MODE_OUTPUT_PP - Push-pull output or GPIO_MODE_INPUT
	GPIO_InitStruct.Speed = speed; //GPIO_SPEED_FREQ_LOW or GPIO_NOPULL
	HAL_GPIO_Init(dht11_sensor->dht11_pin->gpio_port, &GPIO_InitStruct); //GPIOA
}

DHT11_t dht11_init(GPIO_dht11 *dht11_pin, TIM_HandleTypeDef *htim_usdelay)
{
	DHT11_t dht11_dev;

	dht11_dev.htim_usdelay = htim_usdelay;
	dht11_dev.dht11_pin = dht11_pin;

	return dht11_dev;
}

uint8_t dht11_response(DHT11_t *dht11_sensor)
{
	uint8_t response = 0;
	delay_us(dht11_sensor->htim_usdelay, 40);
	if (!HAL_GPIO_ReadPin(dht11_sensor->dht11_pin->gpio_port, dht11_sensor->dht11_pin->gpio_pin)) {
		delay_us(dht11_sensor->htim_usdelay, 80);
	    if (HAL_GPIO_ReadPin(dht11_sensor->dht11_pin->gpio_port, dht11_sensor->dht11_pin->gpio_pin)) response = 1;
	    delay_us(dht11_sensor->htim_usdelay, 80);
	}
	return response;
}

static uint8_t dht11_read_byte(DHT11_t *dht11_sensor)
{
    uint8_t value = 0;
    for (int i = 0; i < 8; i++)
    {
        // Wait for 50 µs LOW
        while (HAL_GPIO_ReadPin(dht11_sensor->dht11_pin->gpio_port, dht11_sensor->dht11_pin->gpio_pin) == GPIO_PIN_RESET);

        // Measure HIGH pulse width
        delay_us(dht11_sensor->htim_usdelay, 30);
        if(HAL_GPIO_ReadPin(dht11_sensor->dht11_pin->gpio_port, dht11_sensor->dht11_pin->gpio_pin) == GPIO_PIN_SET)
        	value = (value << 1) | 1;
        else
        	value = (value << 1);

        while(HAL_GPIO_ReadPin(dht11_sensor->dht11_pin->gpio_port, dht11_sensor->dht11_pin->gpio_pin) == GPIO_PIN_SET);
    }
    return value;
}

uint8_t dht11_read_data(DHT11_t *dht11_sensor)
{
    set_pin_mode(dht11_sensor, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_LOW);
    HAL_GPIO_WritePin(dht11_sensor->dht11_pin->gpio_port, dht11_sensor->dht11_pin->gpio_pin, GPIO_PIN_RESET); 	//Pull high to signal the DHT sensor
    HAL_Delay(20); 											//Delay for at least 18ms
    HAL_GPIO_WritePin(dht11_sensor->dht11_pin->gpio_port, dht11_sensor->dht11_pin->gpio_pin, GPIO_PIN_SET);		//Pull low and wait for response 30us
    delay_us(dht11_sensor->htim_usdelay,30);
    set_pin_mode(dht11_sensor, GPIO_MODE_INPUT, GPIO_NOPULL);

    if (dht11_response(dht11_sensor)) {
    	dht11_sensor->hum_int  = dht11_read_byte(dht11_sensor);
    	dht11_sensor->hum_dec  = dht11_read_byte(dht11_sensor);
    	dht11_sensor->temp_int = dht11_read_byte(dht11_sensor);
    	dht11_sensor->temp_dec = dht11_read_byte(dht11_sensor);
    	dht11_sensor->checksum = dht11_read_byte(dht11_sensor);

        if ((dht11_sensor->hum_int + dht11_sensor->hum_dec + dht11_sensor->temp_int + dht11_sensor->temp_dec) == dht11_sensor->checksum) {
            return 1; // OK
        }
    }
    return 0; // Error
}

