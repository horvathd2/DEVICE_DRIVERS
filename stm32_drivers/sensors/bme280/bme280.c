/*
 * bme280.c
 *
 *  Created on: Jan 25, 2026
 *      Author: H.Dani
 */

#include "bme280.h"
#include "main.h"

BME280_t bme280_init(COM_protocol_t com_prot)
{
	BME280_t bme280_dev = {0};

	bme280_dev.bme280_com = com_prot;

	return bme280_dev;
}
