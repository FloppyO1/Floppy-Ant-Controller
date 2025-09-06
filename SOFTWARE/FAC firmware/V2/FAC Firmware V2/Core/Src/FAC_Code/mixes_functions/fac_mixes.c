/*
 * fac_mixes.c
 *
 *  Created on: Aug 14, 2025
 *      Author: filippo-castellan
 */

#include "FAC_Code/mixes_functions/fac_mixes.h"
//#include "FAC_Code/fac_std_receiver.h"
#include "FAC_Code/fac_settings.h"
#include "main.h"

//#include "FAC_Code/mixes_functions/fac_<name>_mix.h"			// 6) of HOW TO MAKE A MIX

static Mixes mixes;
/* STATIC FUNCTION PROTORYPES */
static void FAC_mixes_SET_current_mix(uint8_t currentMix);
static void FAC_mixes_SET_input(uint8_t inputNumber, float inputValue);
static void FAC_mixes_SET_input_channel_number(uint8_t inputNumber, uint8_t inputChannel);
static void FAC_mixes_SET_output(uint8_t outputNumber, float value);
static void FAC_mixes_SET_input_reversed(uint8_t inputNumber, uint8_t isReversed);
static uint8_t FAC_mixes_GET_current_mix();
static uint8_t FAC_mixes_GET_input_channel_number(uint8_t inputNumber);
static int16_t FAC_mixes_GET_output(uint8_t outputNumber);
static uint8_t FAC_mixes_GET_input_reversed(uint8_t inputNumber);

/* FUNCTION DEFINITION */
/* ----------------------PRIVATE FUNCTIONS---------------------- */
/* @brief	getters and setters
 *
 */
static void FAC_mixes_SET_current_mix(uint8_t currentMix) {
	mixes.current_mix = currentMix;
}

static void FAC_mixes_SET_input(uint8_t inputNumber, float inputValue) {
	mixes.mix_input[inputNumber] = inputValue;
}

static void FAC_mixes_SET_input_channel_number(uint8_t inputNumber, uint8_t inputChannel) {
	mixes.mix_input_channel_number[inputNumber] = inputChannel;
}

static void FAC_mixes_SET_output(uint8_t outputNumber, float value) {
	mixes.mix_output[outputNumber] = value;
}

static void FAC_mixes_SET_input_reversed(uint8_t inputNumber, uint8_t isReversed) {
	mixes.mix_input_reversed[inputNumber] = isReversed;
}

static uint8_t FAC_mixes_GET_current_mix() {
	return mixes.current_mix;
}

static uint8_t FAC_mixes_GET_input_channel_number(uint8_t inputNumber) {
	return mixes.mix_input_channel_number[inputNumber];
}

static int16_t FAC_mixes_GET_output(uint8_t outputNumber) {
	return mixes.mix_output[outputNumber];
}

static uint8_t FAC_mixes_GET_input_reversed(uint8_t inputNumber) {
	return mixes.mix_input_reversed[inputNumber];
}

/* ----------------------PUBBLIC FUNCTIONS---------------------- */

float FAC_mixes_GET_input(uint8_t inputNumber) {
	return mixes.mix_input[inputNumber];
}

/*
 * @brief		Take all the outputs value given by the mix and copy them into the mixes output array
 * @IMPORTANT	!!!! MUST BE CALLED AFTER EACH CALL OF THE MIX UPDATE !!!!
 * @note		this function will not modify the value of the single mix output array
 */
void FAC_mixes_update_mix_outputs(float mix_output[]) {
	for (int i = 0; i < MIXES_MAX_OUTPUTS_NUMBER; i++) {
		FAC_mixes_SET_output(i, mix_output[i]);
	}
}

/*
 * @brief		Take all the input value and calculate the normalized value
 * @IMPORTANT	!!!! MUST BE CALLED AFTER EACH CALL OF THE MIX UPDATE !!!!
 * @note		take in input the settings input array from settings
 */
void FAC_mixes_update_mix_inputs() {
	for (int i = 0; i < MIXES_MAX_INPUTS_NUMBER; i++) {
		uint8_t chNumber = FAC_mixes_GET_input_channel_number(i);	// get channel number corresponding to the input evaluated

		if (chNumber != 0) {	// if this channel is valid
			uint16_t receiverResolution = FAC_settings_GET_value(FAC_SETTINGS_CODE_RECEIVER_RESOLUTION);
			float chValue = (float) (FAC_std_receiver_GET_channel(chNumber)) / (float) (receiverResolution);	// get the receiver channel value
			float inputValue = map_float(chValue, 0.0f, 1.0f, -1.0f, 1.0f); // map the channel value to make it standard [-1.0 to 1.0]

			// reverse input if it is reversed
			if(FAC_settings_GET_value(FAC_SETTINGS_CODE_MIX_INPUT1_REVERSED + i)) inputValue = inputValue * (-1.0f) ;

			FAC_mixes_SET_input(i, inputValue);	// store the value into the struct array (where all mixes will take them)
		}
	}
}

/*
 *
 *
 */
void FAC_mix_update() {							// 5) of HOW TO MAKE A MIX
	switch (FAC_mixes_GET_current_mix()) {
		case FAC_MIX_NONE:

			break;
		case FAC_MIX_SIMPLE_TANK:

			break;
		/*
		case FAC_MIX_<NAME>:
			FAC_<name>_mix_update();
		break;
		*/
	}
}

/*
 * @brief		Initialize the mixes struct
 * @IMPORTANT	!! BEFORE CALLING THIS FUNCTION MAKE SECURE TO LOAD SETTINGS FROM EEPROM !!
 * @note		initialized to zero (all disabled all mix input and output) get the channels of all inputs
 */
void FAC_mixes_init() {
	FAC_mix_SET_current_mix(FAC_settings_GET_value(FAC_SETTINGS_CODE_ACTIVE_MIX));	// get the active mix from the settings

	for (int i = 0; i < MIXES_MAX_INPUTS_NUMBER; i++) {	// set all input channels to zero (no input selected)
		FAC_mixes_SET_input_channel_number(i, FAC_settings_GET_value(FAC_SETTINGS_CODE_MIX_INPUT1_CHANNEL + i));// take from settings the inputvalues

		mixes.mix_input[i] = 0.0f;
		mixes.mix_input_reversed[i] = FALSE;
	}

	for (int i = 0; i < MIXES_MAX_OUTPUTS_NUMBER; i++) {	// set all output to 0 and set them to non reversed
		mixes.mix_output[i] = 0;
	}
}

/*
 * AFTER THIS THERE IS ALL OF THE OLD METHOD
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */

///* STATIC FUNCTION PROTORYPES */
//static void FAC_mix_SET_current_mix(uint8_t currentMix);
///* FUNCTION DEFINITION */
///* ----------------------PRIVATE FUNCTIONS---------------------- */
//static void FAC_mix_SET_current_mix(uint8_t currentMix) {
//	mix.current_mix = currentMix;
//}
///* ----------------------PUBBLIC FUNCTIONS---------------------- */
//
//uint8_t FAC_mix_GET_current_mix() {
//	return mix.current_mix;
//}
//
///*
// * @brief	This function calculate the new value applying a deadzone in the center (0) and at the extremes
// * @note 	Center deadzone applied only in case the value is positive and negative.
// *
// *               __
// *              /	 <-- high deadzone
// *             /
// *            /
// *           /
// *         --		<-- center deadzone
// *        /
// *       /
// *      /
// *   __/			<-- low deadzone
// *
// */
//int16_t FAC_mix_calculate_dead_zone(int16_t value, uint8_t deadzonePerc, int16_t minValue, int16_t maxValue) {
//	int16_t temp = (int16_t) value;
//	uint16_t deadzoneValue = ((maxValue - minValue) / 100) * deadzonePerc; // calculate the deadzone using the precentage argument on the full range
//	/* keep the value inside the range */
//	if (value > maxValue)
//		temp = maxValue;
//	if (value < minValue)
//		temp = minValue;
//
//	// extremes deadzone
//	if (temp + deadzoneValue >= maxValue)
//		return maxValue;
//	if (temp - deadzoneValue <= minValue)
//		return minValue;
//
//	if (temp >= -deadzoneValue && temp <= deadzoneValue)
//		return 0;	// the value is inside the center deadzone
//
//	/* linearization of the value if it is not in the center */
//	if (temp > 0)	// linearize the top value
//		return map_int32(temp, 0 + deadzoneValue, maxValue - deadzoneValue, 0, maxValue);
//	if (temp < 0)	// linearize the bottom value
//		return map_int32(temp, minValue + deadzoneValue, 0 - deadzoneValue, minValue, 0);
//
//	return temp;	// if something go wrong (no case detected return the value without deadzone
//}
//
///*
// * @brief	Update the current mix, and apply the update to servos and motors
// *
// */
//void FAC_mix_update() {
//	uint8_t currentMix = FAC_mix_GET_current_mix();
//	switch (currentMix) {
//		case FAC_MIX_NONE:
//			FAC_none_mix_update();
//			break;
//		case FAC_MIX_SIMPLE_TANK:
//			FAC_tank_mix_update();
//			break;
//	}
//}
//
///*
// * @brief		Initialize the correct mix according to the active one
// * @IMPORTANT	!! MUST BE CALLED IF NEW SETTINGS ARE LOADED !!
// * @note		Insert here a case for each new mix (import the mix file)
// */
//void FAC_mix_init(uint8_t currentMix, uint8_t deadzonePerc) {
//	FAC_mix_SET_current_mix(currentMix);
//	switch (currentMix) {
//		case FAC_MIX_NONE:
//			FAC_none_mix_init(deadzonePerc, 1, 2, 5, 3, 4);
//			break;
//		case FAC_MIX_SIMPLE_TANK:
//			FAC_tank_mix_init(2, 1, 1, 2, 0, 0, deadzonePerc);	//thch 1, stch 2, lmotor 1, rmotor 2, th no reversed, st no reversed, deadzone 1%
//			break;
//	}
//}
