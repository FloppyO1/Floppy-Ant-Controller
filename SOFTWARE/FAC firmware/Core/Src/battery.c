/*
 * battery.c
 *
 *  Created on: Jul 17, 2024
 *      Author: Filippo Castellan
 */

#include "battery.h"

extern ADC_HandleTypeDef hadc;

void initBattery() {
	HAL_ADC_Init(&hadc);
	getADCValue(BATTERY_CHANNEL);	// make the first conversion because it always returns 0
}

/* return: 	- 1 if 1S
 * 			- 2 if 2s
 * 			- 0 if none of above
 */
uint8_t getBatteryConfiguration() {
	uint16_t vbat = getBattVoltage();
	if (vbat >= MIN_1S_VOLTAGE && vbat <= MAX_1S_VOLTAGE) return 1;	// add (or subtracted) 200 to have some margin
	if (vbat >= MIN_2S_VOLTAGE && vbat <= MAX_2S_VOLTAGE) return 2;	// add (or subtracted) 200 to have some margin
	return 0;
}

uint16_t getADCValue(uint8_t channel) {
	uint16_t adc;
	HAL_ADC_Start(&hadc);
	adc = HAL_ADC_GetValue(&hadc);
	HAL_ADC_Stop(&hadc);
	return adc;
}

uint16_t getBattVoltage() {
	const uint32_t vref = 3300000;		// 3.3V in uV to make the result an integer
	const uint32_t adcRes = 4096;		// 12 bit resolution
	const uint32_t dividerRatio = 3;	// voltage divider in the circuit is 1/3
	uint32_t adc = getADCValue(BATTERY_CHANNEL);
	uint32_t vbat = ((vref / adcRes) * adc * dividerRatio) / 1000;	// /1000 to have the value in mV
	return vbat + COMPENSATION_VALUE;
}
