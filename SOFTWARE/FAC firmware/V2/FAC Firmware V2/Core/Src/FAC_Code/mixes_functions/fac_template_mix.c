/*
 * fac_template_mix.c
 *
 *	HOW TO MAKE A MIX:
 *	1) Copy this file and the fac_template_mix.h and rename with the name of the new mix (ex: fac_simple_tank_mix.c/h)
 *	2) Fix the include to match the new name you have choose
 *	3) Add to the FAC_MIXES_ID on fac_mixes.h file the new FAC_MIX (ex: FAC_MIX_SIMPLE_TANK) and set it to the mix_id
 *	4) Rename the FAC_<name>_mix_update() function on this file and on the fac_name_mix.h
 *	5) Add the FAC_<name>_mix_update() function into a new case into the fac_mixes.c file inside the FAC_mix_update() function using as case value your mix_id value.
 *	6) Add the include the header file of your new mix inside the fac_mixes.c (#include "FAC_Code/mixes_functions/fac_<name>_mix.h")
 *	7) Make a description of the mix and describe all inputs/outputs it takes/gives
 *		Remeber: an input is a remote controller channel converted in standard values (the channel is chosen by the settings). If for example you need the gyroscope data you have to manage it inside your code
 *	8) Include all the modules you need for your mix logic (uncomment the modules you need in CUSTOM MIX INCLUDE part)
 *	9) Write your mix logic code
 *  Created on: Sep 5, 2025
 *      Author: filippo-castellan
 */

#include "FAC_Code/mixes_functions/fac_template_mix.h"			// 2) of HOW TO MAKE A MIX

/* DEFAULT INCLUDE */ // DON´T TOUCH THIS CODE!!
#include "FAC_Code/mixes_functions/fac_mixes.h"
#include "FAC_Code/fac_settings.h"

/* CUSTOM MIX INCLUDE */										// 8) of HOW TO MAKE A MIX
//#include "FAC_Code/fac_adc.h"
//#include "FAC_Code/fac_gyro.h"


/* PRIVATE FUNCTIONS AND VARIABLES */
static const uint8_t mix_id = FAC_MIX_NAME;						// 3) of HOW TO MAKE A MIX		(only to know witch mix is this)

/* WHAT THIS MIX DO */											// 7) of HOW TO MAKE A MIX
/*
 * DESCRIPTION:
 * write here the description of your mix
 *
 * INPUTs DESCRIPTION:
 * write here the input type that this mix must receive (example 1) throttle, 2) steering, 3) not used)
 * 0) not used
 * 1) not used
 * 2) not used
 * 3) not used
 * 4) not used
 * 5) not used
 * 6) not used
 * 7) not used
 *
 * OUTPUTs DESCRIPTION:
 * !! REMEMBER:
 * 		- All outputs must be in a standard format, values form -RECEIVER_CHANNEL_RESOLUTION to RECEIVER_CHANNEL_RESOLUTION !!
 * 			write here what each mix's output is (ex: 0) motor left, 1) motor right 2)not used ...)
 * 		- If an output is for a DC motor, a positive number is considered as forward movement, a negative number is considered as backwards movement of the DC motor
 * 		- If an output is for a Servo vomor/esc, -1.0 is considered as 0 and 1.0 is considered as 100% (in degrees for servos 0°-180°)
 * 0) not used
 * 1) not used
 * 2) not used
 * 3) not used
 * 4) not used
 * 5) not used
 * 6) not used
 * 7) not used
 * 8) not used
 * 9) not used
 */

/*
 * @brief	Calculate the mix output values
 *
 */
void FAC_name_mix_update() {
	// this code must be left as it is, DON'T TOUCH IT!
	float outputs[MIXES_MAX_OUTPUTS_NUMBER];
	float inputs[MIXES_MAX_INPUTS_NUMBER];
	FAC_mixes_update_mix_inputs();	// update the mix input in base of the settings and rx channels
	for (int i = 0; i < MIXES_MAX_INPUTS_NUMBER; i++) {
		inputs[i] = FAC_mixes_GET_input(i);
	}
	/* INSERT YOUR CODE HERE -START- */						// 9) of HOW TO MAKE A MIX
	/* REMEMBER
	 * - inputs array contains all values of the channel requested for this mix
	 * - in the outputs array you have to writhe in the same order you written above all outputs for servos and motors
	 * 		outputs starts must stay in this range [-1.0, +1.0]
	 */
	// write here the code of your mix
	if (0) {	//delete this and insert your code

	}
	/* INSERT YOUR CODE HERE -END- */
	// keep outputs in range
	for (int i = 0; i < MIXES_MAX_OUTPUTS_NUMBER; i++) {
		if (outputs[i] > 1.0f)
			outputs[i] = 1.0f;
		if (outputs[i] < -1.0f)
			outputs[i] = -1.0f;
	}
	// update outputs values on mixes struct
	FAC_mixes_update_mix_outputs(outputs);
}
