/*
 * fac_functions.c
 *
 *  Created on: Sep 6, 2025
 *      Author: filippo-castellan
 */

#include "FAC_Code/mixes_functions/fac_functions.h"
#include "FAC_Code/fac_settings.h"
#include "main.h"

SpecialFunctions sFunctions;

/* STATIC FUNCTION PROTORYPES */

/* FUNCTION DEFINITION */
/* ----------------------PRIVATE FUNCTIONS---------------------- */
/*
 * @brief	getter and setter functions
 */
static void FAC_functions_SET_input_channels(uint8_t functionNumber, uint8_t inputChannel) {
	sFunctions.special_functions_input_channels[functionNumber] = inputChannel;
}

static void FAC_functions_SET_input(uint8_t functionNumber, float inputValue) {
	sFunctions.special_functions_inputs[functionNumber] = inputValue;
}

static void FAC_functions_SET_output(uint8_t functionNumber, float outputValue) {
	sFunctions.special_functions_outouts[functionNumber] = outputValue;
}

static uint8_t FAC_functions_GET_input_channels(uint8_t functionNumber) {
	return sFunctions.special_functions_input_channels[functionNumber];
}

static float FAC_functions_GET_input(uint8_t functionNumber) {
	return sFunctions.special_functions_inputs[functionNumber];
}

static float FAC_functions_GET_output(uint8_t functionNumber) {
	return sFunctions.special_functions_outouts[functionNumber];
}

/* ----------------------PUBBLIC FUNCTIONS---------------------- */

/*
 * @brief	this function update the special function output corresponding to the id given
 * @note	Some function can be used for multiple times (ex: direct link to channel, each id is a different function input for the same algorithm)
 */
void fac_finctions_update(uint8_t sFunctionID) {
	switch (sFunctionID) {
		case FAC_SPECIAL_FUNCTION_DIRECT_LINK_TO_CHANNEL:

			break;
		/*
		case FAC_SPECIAL_FUNCTION_<NAME>:
			FAC_<name>_function_update(sFunctionID);
			break;
		*/
	}
}

/*
 * @brief		Initialize the special functions struct
 * @IMPORTANT	!! BEFORE CALLING THIS FUNCTION MAKE SECURE TO LOAD SETTINGS FROM EEPROM !!
 * @note		initialized to zero (all disabled all mix input and output)
 */
void FAC_functions_init() {
	for (int i = 0; i < SPECIAL_FUNCITONS_NUMBER; i++) {	// set all input channels to zero (no input selected)
		FAC_functions_SET_input_channels(i, FAC_settings_GET_value(FAC_SETTINGS_CODE_SPECIAL_FUNCTION1_INPUT_CHANNEL + i));
		FAC_functions_SET_input(i, 0.0f);
		FAC_functions_SET_output(i, 0.0f);
	}
}

