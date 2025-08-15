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
	uint8_t motors_channels[MOTORS_NUMBER];  // only one channel for each servo/motor, viceversa one channel can have multiple motors/servos
	uint8_t servos_channels[SERVOS_NUMBER];
	uint8_t dead_zone;	// deadzone expressed in percentage for commands steering and throttle
} Nonemix;

void FAC_none_mix_init(uint8_t deadzone, uint8_t m1Ch, uint8_t m2Ch, uint8_t m3Ch, uint8_t s1Ch, uint8_t s2Ch);
void FAC_none_mix_update();

#endif /* INC_FAC_CODE_MIXES_FAC_NONE_MIX_H_ */
