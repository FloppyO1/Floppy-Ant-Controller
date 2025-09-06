/*
 * fac_functions.h
 *
 *  Created on: Sep 6, 2025
 *      Author: filippo-castellan
 */

#ifndef INC_FAC_CODE_MIXES_FUNCTIONS_FAC_FUNCTIONS_H_
#define INC_FAC_CODE_MIXES_FUNCTIONS_FAC_FUNCTIONS_H_

#include "stm32f0xx_hal.h"
#include "FAC_Code/config.h"

#define SPECIAL_FUNCITONS_NUMBER 20

typedef struct SpecialFunctions {
		uint8_t special_functions_input_channels[SPECIAL_FUNCITONS_NUMBER];	// 20 possible function, each function has its own input channel
		float special_functions_inputs[SPECIAL_FUNCITONS_NUMBER];		// 20 normalized inputs one for each function
		float special_functions_outouts[SPECIAL_FUNCITONS_NUMBER];		// 20 normalized outputs one for each function
} SpecialFunctions;


enum FAC_SPECIAL_FUNCTIONS_ID {
	FAC_SPECIAL_FUNCTION_NAME,
	FAC_SPECIAL_FUNCTION_DIRECT_LINK_TO_CHANNEL,


	FAC_SPECIAL_FUNCTION_LAST
};

#endif /* INC_FAC_CODE_MIXES_FUNCTIONS_FAC_FUNCTIONS_H_ */
