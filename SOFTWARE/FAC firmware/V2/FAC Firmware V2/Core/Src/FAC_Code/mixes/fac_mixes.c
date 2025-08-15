/*
 * fac_mixes.c
 *
 *  Created on: Aug 14, 2025
 *      Author: filippo-castellan
 */

#include "FAC_Code/mixes/fac_mixes.h"
#include "FAC_Code/mixes/fac_none_mix.h"
#include "FAC_Code/mixes/fac_tank_mix.h"

static Mix mix;

/* STATIC FUNCTION PROTORYPES */
static void FAC_mix_SET_current_mix(uint8_t currentMix);
/* FUNCTION DEFINITION */
/* ----------------------PRIVATE FUNCTIONS---------------------- */
static void FAC_mix_SET_current_mix(uint8_t currentMix) {
	mix.current_mix = currentMix;
}
/* ----------------------PUBBLIC FUNCTIONS---------------------- */

uint8_t FAC_mix_GET_current_mix() {
	return mix.current_mix;
}

/*
 * @brief	This function calculate the new value applying a deadzone in the center (0) and at the extremes
 * @note 	Center deadzone applied only in case the value is positive and negative.
 *
 *               __
 *              /	 <-- high deadzone
 *             /
 *            /
 *           /
 *         --		<-- center deadzone
 *        /
 *       /
 *      /
 *   __/			<-- low deadzone
 *
 */
int16_t FAC_mix_calculate_dead_zone(int16_t value, uint8_t deadzonePerc, int16_t minValue, int16_t maxValue) {
	int16_t temp = (int16_t) value;
	uint16_t deadzoneValue = ((maxValue - minValue) / 100) * deadzonePerc;	// calculate the deadzone using the precentage argument on the full range
	/* keep the value inside the range */
	if (value > maxValue)
		temp = maxValue;
	if (value < minValue)
		temp = minValue;

	// extremes deadzone
	if (temp + deadzoneValue >= maxValue)
		return maxValue;
	if (temp - deadzoneValue <= minValue)
		return minValue;

	if (temp >= -deadzoneValue && temp <= deadzoneValue)
		return 0;	// the value is inside the center deadzone

	/* linearization of the value if it is not in the center */
	if (temp > 0)	// linearize the top value
		return map_int32(temp, 0 + deadzoneValue, maxValue - deadzoneValue, 0, maxValue);
	if (temp < 0)	// linearize the bottom value
		return map_int32(temp, minValue + deadzoneValue, 0 - deadzoneValue, minValue, 0);

	return temp;	// if something go wrong (no case detected return the value without deadzone
}

/*
 * @brief	Update the current mix, and apply the update to servos and motors
 *
 */
void FAC_mix_update() {
	uint8_t currentMix = FAC_mix_GET_current_mix();
	switch (currentMix) {
		case FAC_MIX_NONE:
			FAC_none_mix_update();
			break;
		case FAC_MIX_SIMPLE_TANK:
			FAC_tank_mix_update();
			break;
	}
}

/*
 * @brief		Initialize the correct mix according to the active one
 * @IMPORTANT	!! MUST BE CALLED IF NEW SETTINGS ARE LOADED !!
 * @note		Insert here a case for each new mix (import the mix file)
 */
void FAC_mix_init(uint8_t currentMix, uint8_t deadzonePerc) {
	FAC_mix_SET_current_mix(currentMix);
	switch (currentMix) {
		case FAC_MIX_NONE:
			FAC_none_mix_init(deadzonePerc, 1, 2, 5, 3, 4);
			break;
		case FAC_MIX_SIMPLE_TANK:
			FAC_tank_mix_init(2, 1, 1, 2, 0, 0, deadzonePerc);	//thch 1, stch 2, lmotor 1, rmotor 2, th no reversed, st no reversed, deadzone 1%
			break;
	}
}
