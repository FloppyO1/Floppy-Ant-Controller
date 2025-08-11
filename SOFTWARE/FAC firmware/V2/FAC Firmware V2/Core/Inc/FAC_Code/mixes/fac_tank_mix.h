/*
 * fac_tank_mix.h
 *
 *  Created on: Aug 11, 2025
 *      Author: filippo-castellan
 */

#ifndef INC_FAC_CODE_MIXES_FAC_TANK_MIX_H_
#define INC_FAC_CODE_MIXES_FAC_TANK_MIX_H_

#include "stm32f0xx_hal.h"
#include "FAC_Code/mixes/fac_mixes.h"

typedef struct Tankmix {
	uint8_t throttle_channel;	// (mixch1 in settings) receiver channel associated to the throttle
	uint8_t steering_channel;	// (mixch2 in settings) receiver channel associated to the steering
	uint8_t left_motor;			// left motor number
	uint8_t right_motor;		// right motor number
	uint8_t throttle_reversed;	// TRUE if the throttle is reversed
	uint8_t steering_reversed;	// TRUE if the steering is reversed
	uint16_t left_motor_speed;
	uint16_t right_motor_speed;
	uint8_t left_motor_direction;
	uint8_t right_motor_direction;
} Tankmix;

#endif /* INC_FAC_CODE_MIXES_FAC_TANK_MIX_H_ */
