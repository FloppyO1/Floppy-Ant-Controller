/*
 * battery.h
 *
 *  Created on: Jul 17, 2024
 *      Author: Filippo Castellan
 */

#ifndef INC_BATTERY_H_
#define INC_BATTERY_H_

#include "stm32f0xx_hal.h"

#define BATTERY_CHANNEL 0
#define ADC1_CHANNEL 1
#define ADC2_CHANNEL 2

#define COMPENSATION_VALUE 100 // value added to the reading to have the exact voltage

#define MIN_1S_VOLTAGE 2800
#define MAX_1S_VOLTAGE 4200

#define MIN_2S_VOLTAGE MIN_1S_VOLTAGE*2
#define MAX_2S_VOLTAGE MAX_1S_VOLTAGE*2

void initBattery();
uint16_t getADCValue(uint8_t channel);
uint16_t getBattVoltage();
uint8_t getBatteryConfiguration();



#endif /* INC_BATTERY_H_ */
