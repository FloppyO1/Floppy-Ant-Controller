/*
 * fax_mixes.h
 *
 *	HOW TO MAKE A CUSTOM MIX:
 *	Write here how to..
 *
 *
 *  Created on: Aug 11, 2025
 *      Author: filippo-castellan
 */

#ifndef INC_FAC_CODE_MIXES_FAC_MIXES_H_
#define INC_FAC_CODE_MIXES_FAC_MIXES_H_

#include "FAC_Code/config.h"
#include "FAC_Code/fac_std_receiver.h"
#include "FAC_Code/fac_motors.h"
#include "FAC_Code/fac_servo.h"
#include <stdlib.h>

/* needed for the deadzone calculation, declared in fac_app.h */
extern uint32_t map_uint32(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max);


typedef struct Mix{
	uint8_t current_mix;
} Mix;

enum FAC_MIXES{
	FAC_MIX_NONE,
	FAC_MIX_SIMPLE_TANK
};


int16_t FAC_mix_calculate_dead_zone(int16_t value, uint8_t deadzonePerc, int16_t minValue, int16_t maxValue);

#endif /* INC_FAC_CODE_MIXES_FAC_MIXES_H_ */
