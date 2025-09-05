/*
 * fac_template_mix.c
 *
 *	HOW TO MAKE A MIX:
 *	1) Copy this file and the fac_template_mix.h and rename with the name of the new mix (ex: fac_simple_tank_mix.c/h)
 *	2) Fix the include to match the new name you have choose
 *	3) Add to the FAC_MIXES_ID on fac_mixes.h file the new FAC_MIX (ex: FAC_MIX_SIMPLE_TANK) and set it to the mix_id
 *	4) Rename the FAC_<name>_mix_update_outputs() function on this file and on the fac_name_mix.h
 *  Created on: Sep 5, 2025
 *      Author: filippo-castellan
 */

#include "FAC_Code/mixes_functions/fac_template_mix.h"		// 2) of HOW TO MAKE A MIX

/* DEFAULT INCLUDE */ // DON´T TOUCH THIS CODE!!
#include "FAC_Code/mixes_functions/fac_mixes.h"
#include "FAC_Code/fac_settings.h"

/* PRIVATE FUNCTIONS AND VARIABLES */
static const uint8_t mix_id = FAC_MIX_NAME;						// 3) of HOW TO MAKE A MIX		(only to know witch mix is this)

/* WHAT THIS MIX DO */
/*
 * DESCRIPTION:
 * write here the description of your mix
 *
 * INPUT CHANNELs TAKEN:
 * write here the channel type that this mix must receive (example 1) throttle, 2) steering, 3) not used )
 * 1) not used
 * 2) not used
 * 3) not used
 * 4) not used
 * 5) not used
 * 6) not used
 * 7) not used
 * 8) not used
 *
 * OUTPUTs DESCRIPTION:
 * !! REMEMBER: all outputs must be in a standard format, values form -RECEIVER_CHANNEL_RESOLUTION to RECEIVER_CHANNEL_RESOLUTION !!
 * write here what each mix's output is (ex: 1) motor left, 2) motor right 3)not used ...)
 * 1) not used
 * 2) not used
 * 3) not used
 * 4) not used
 * 5) not used
 * 6) not used
 * 7) not used
 * 8) not used
 * 9) not used
 * 10) not used
 */

/*
 * @brief	Calculate the mix output values
 *
 */
uint8_t FAC_name_mix_update_outputs() {
	// this code must be left as it is, DON'T TOUCH IT!
	int16_t outputs[MIXES_MAX_OUTPUTS_NUMBER];
	int16_t inputs[MIXES_MAX_INPUTS_NUMBER];
	FAC_mixes_update_mix_inputs();	// update the mix input in base of the settings and rx channels
	for (int i = 0; i < MIXES_MAX_INPUTS_NUMBER; i++) {
		inputs[i] = FAC_mixes_GET_input(i);
	}
	/* INSERT YOUR CODE HERE -START- */
	// write here the code of your mix
	/* INSERT YOUR CODE HERE -END- */
	// keep outputs in range
	uint16_t receiverResolution = FAC_settings_GET_value(FAC_SETTINGS_CODE_RECEIVER_RESOLUTION);
	for (int i = 0; i < MIXES_MAX_OUTPUTS_NUMBER; i++) {
		if (outputs[i] > receiverResolution - 1)
			outputs[i] = receiverResolution - 1;
		if (outputs[i] < -receiverResolution + 1)
			outputs[i] = -receiverResolution + 1;
	}
	// update outputs values on mixes struct
	FAC_mixes_update_mix_outputs(outputs);
}
