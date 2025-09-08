/*
 * fac_functions.c
 *
 *  Created on: Sep 6, 2025
 *      Author: filippo-castellan
 */

#include "FAC_Code/mixes_functions/fac_functions.h"
#include "FAC_Code/fac_app.h"
#include "FAC_Code/fac_std_receiver.h"
#include "FAC_Code/fac_settings.h"
#include "main.h"

SpecialFunctions sFunctions;

/* NEW SPECIAL FUNCTIONS */
//#include "FAC_Code/mixes_functions/functions/fac_<name>_function.h"			// 6) of HOW TO MAKE A MIX
#include  "FAC_Code/mixes_functions/functions/fac_direct_link_function.h"

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

static uint8_t FAC_functions_GET_input_channel_number(uint8_t functionNumber) {
	return sFunctions.special_functions_input_channels[functionNumber];
}

/* ----------------------PUBBLIC FUNCTIONS---------------------- */

float FAC_functions_GET_output(uint8_t functionNumber) {
	return sFunctions.special_functions_outouts[functionNumber];
}

float FAC_functions_GET_input(uint8_t functionNumber) {
	return sFunctions.special_functions_inputs[functionNumber];
}

void FAC_functions_SET_output(uint8_t functionNumber, float outputValue) {
	sFunctions.special_functions_outouts[functionNumber] = outputValue;
}

/*
 * @brief		Take all the input value and calculate the normalized value
 * @IMPORTANT	!!!! MUST BE CALLED AFTER EACH CALL OF THE FUCTIONS UPDATE !!!!
 * @note		take in input the settings input array from settings
 */
void FAC_functions_update_inputs() {
//	for (int i = 0; i < SPECIAL_FUNCITONS_NUMBER; i++) {
//		uint8_t chNumber = FAC_functions_GET_input_channel_number(i);	// get channel number corresponding to the input evaluated
//
//		if (chNumber != 0) {
//			uint16_t receiverResolution = FAC_settings_GET_value(FAC_SETTINGS_CODE_RECEIVER_RESOLUTION);
//			float chValue = (float) (FAC_std_receiver_GET_channel(chNumber)) / (float) (receiverResolution);	// get the receiver channel value
//			float inputValue = map_float(chValue, 0.0f, 1.0f, -1.0f, 1.0f); // map the channel value to make it standard [-1.0 to 1.0]
//
//			FAC_functions_SET_input(i, inputValue);	// store the value into the struct array (where all mixes will take them)
//		}
//	}
	for (int i = 0; i < SPECIAL_FUNCITONS_NUMBER; i++) {
		uint8_t chNumber = FAC_functions_GET_input_channel_number(i);	// get channel number corresponding to the input evaluated

		if (chNumber != 0) {	// if this channel is valid
			uint16_t receiverResolution = FAC_settings_GET_value(FAC_SETTINGS_CODE_RECEIVER_RESOLUTION);
			uint16_t rxValue = FAC_std_receiver_GET_channel(chNumber);
			float chValue = (float) (rxValue) / (float) (receiverResolution);	// get the receiver channel value
			float inputValue = map_float(chValue, 0.0f, 1.0f, -1.0f, 1.0f); // map the channel value to make it standard [-1.0 to 1.0]

			FAC_functions_SET_input(i, inputValue);	// store the value into the struct array (where all mixes will take them)
		}
	}
}

/*
 * @brief	this function update the special function output corresponding to the id given
 * @note	Some function can be used for multiple times (ex: direct link to channel, each id is a different function input for the same algorithm)
 */
void FAC_functions_update(uint8_t sFunctionID) {
	switch (sFunctionID) {
		case FAC_SPECIAL_FUNCTION_DIRECT_LINK_TO_CHANNEL_1ST:
		case FAC_SPECIAL_FUNCTION_DIRECT_LINK_TO_CHANNEL_2DN:
		case FAC_SPECIAL_FUNCTION_DIRECT_LINK_TO_CHANNEL_3RD:
		case FAC_SPECIAL_FUNCTION_DIRECT_LINK_TO_CHANNEL_4TH:
		case FAC_SPECIAL_FUNCTION_DIRECT_LINK_TO_CHANNEL_5TH:
		case FAC_SPECIAL_FUNCTION_DIRECT_LINK_TO_CHANNEL_6TH:
		case FAC_SPECIAL_FUNCTION_DIRECT_LINK_TO_CHANNEL_7TH:
		case FAC_SPECIAL_FUNCTION_DIRECT_LINK_TO_CHANNEL_8TH:
			FAC_direct_link_function_update(sFunctionID);
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

