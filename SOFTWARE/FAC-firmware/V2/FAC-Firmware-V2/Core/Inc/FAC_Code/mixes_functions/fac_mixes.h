/*
 * fax_mixes.h
 *
 *	HOW MIXes WORKS:
 *	Each mix implements all the mixing logic using the receiver channel
 *	values ​​and/or sensor values ​​as inputs, and writes the outputs in a
 *	standard format [-1.0f ,+1.0f].
 *	to a predefined array in the Mixes structure.
 *	Each generated output from the mix is ​​assigned a value in the array,
 *	which is then taken by the mapper and applied to the appropriate
 *	device selected from the settings.
 *
 *	Each mix writes its output values ​​to the Mixes structure.
 *	This is the structure from which the mapper will take the output
 *	values ​​and map them to the appropriate devices.
 *
 *	HOW TO MAKE A CUSTOM MIX:
 *	Write here how to..
 *
 *
 *  Created on: Aug 11, 2025
 *      Author: filippo-castellan
 */

#ifndef INC_FAC_CODE_MIXES_FUNCTIONS_FAC_MIXES_H_
#define INC_FAC_CODE_MIXES_FUNCTIONS_FAC_MIXES_H_

#include "stm32f0xx_hal.h"
#include "FAC_Code/config.h"

/* needed for the deadzone calculation, declared in fac_app.h */
//extern uint32_t map_uint32(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max);
extern int32_t map_int32(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max);
extern float map_float(float x, float in_min, float in_max, float out_min, float out_max);

#define MIXES_MAX_INPUTS_NUMBER 8
#define MIXES_MAX_OUTPUTS_NUMBER 10

typedef struct Mixes {
	uint8_t current_mix;											// FAC_MIXES_ID of the active mix
	uint8_t mix_input_channels_number[MIXES_MAX_INPUTS_NUMBER];				// input channels number given by settings (chennale number not channel values)
	float mix_input[MIXES_MAX_INPUTS_NUMBER];				// input values (value of the channel not of the channel number) [-1.0f, 1.0f]
	uint8_t mix_input_reversed[MIXES_MAX_INPUTS_NUMBER];			// boolean value that indicates if the INPUT is reversed or not
	float mix_output[MIXES_MAX_OUTPUTS_NUMBER];					// output calculated from the mix's logics [-1.0f, 1.0f]
} Mixes;

enum FAC_MIXES_ID {			// 3) of HOW TO MAKE A MIX
	FAC_MIX_NONE,			// this mix disable all the devices
	FAC_MIX_SIMPLE_TANK,

	FAC_MIX_LAST,
};

float FAC_mixes_GET_output(uint8_t outputNumber);
float FAC_mixes_GET_input(uint8_t inputNumber);
void FAC_mixes_update_mix_inputs();
void FAC_mixes_update_mix_outputs();
void FAC_mix_update();
void FAC_mixes_init();

#endif /* INC_FAC_CODE_MIXES_FUNCTIONS_FAC_MIXES_H_ */
