/*
 * fac_none_mix.h
 *
 *  Created on: Aug 14, 2025
 *      Author: filippo-castellan
 */

#ifndef INC_FAC_CODE_MIXES_FAC_NONE_MIX_H_
#define INC_FAC_CODE_MIXES_FAC_NONE_MIX_H_

#include "stm32f0xx_hal.h"
#include "FAC_Code/mixes/fac_mixes.h"

typedef struct Nonemix {
	uint8_t channel1_motor_number;
	uint8_t channel2_motor_number;
	uint8_t channel3_servo_number;
	uint8_t channel4_servo_number;
	uint8_t channel5_motor_number;
	// ch6 -> Arming (active low)
	// ch7/ch8 not used
	uint8_t dead_zone;	// deadzone expressed in percentage for commands steering and throttle
} Nonemix;

#endif /* INC_FAC_CODE_MIXES_FAC_NONE_MIX_H_ */
