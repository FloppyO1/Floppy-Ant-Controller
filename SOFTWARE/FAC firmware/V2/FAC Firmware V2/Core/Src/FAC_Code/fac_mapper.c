/*
 * fac_mapper.c
 *
 *	HOW MAPPER WORKS:
 *	The mapper reads from the settings which device is linked
 *	to which output of the active mix or special function, and sets
 *	the values ​​of the various outputs to the corresponding devices.
 *	Mix outputs use the prefix 100 to identify them, while special
 *	functions use the prefix 200. (e.g., m1 = 102, motor 1 takes
 *	the value of output 2 of the active mix.)
 *	The mapper takes the active outputs from the active mixes or
 *	functions and conditions the value of the given output
 *	(in standard format) to be understandable by the devices.
 *	If the special function has input channel = 0, it is disabled
 *	and should not be calculated.
 *
 *	The mapper calculates the outputs at the time of the update call (FAC_mapper_apply_to_devices()),
 *	and for each peripheral (if active) it conditions the outputs
 *	and applies them to the peripheral.
 *
 *
 *  Created on: Sep 5, 2025
 *      Author: filippo-castellan
 */

#include "FAC_Code/fac_mapper.h"
#include "FAC_Code/mixes_functions/fac_mixes.h"
#include "FAC_Code/mixes_functions/fac_functions.h"
#include "main.h"

#include "FAC_Code/fac_settings.h"
#include "FAC_Code/fac_motors.h"
#include "FAC_Code/fac_servo.h"

/*
 * @brief		This function apply all outputs to the linked device such as DC motor and servo
 * @IMPORTANT	!! THIS FUNCTION MUST BE CALLED EVERY LOOP TO MANTAIN THE DEVICES STATUS UPDATED !!
 * @note		If a motor/servo is not mapped to any output it will be disabled (DC motors are setted to 0 velocity, breaked, servo pwm is disabled)
 */
void FAC_mapper_apply_to_devices() {
	const uint8_t m1_link = FAC_settings_GET_value(FAC_SETTINGS_CODE_MAPPER_M1);
	const uint8_t m2_link = FAC_settings_GET_value(FAC_SETTINGS_CODE_MAPPER_M2);
	const uint8_t m3_link = FAC_settings_GET_value(FAC_SETTINGS_CODE_MAPPER_M3);
	const uint8_t s1_link = FAC_settings_GET_value(FAC_SETTINGS_CODE_MAPPER_S1);
	const uint8_t s2_link = FAC_settings_GET_value(FAC_SETTINGS_CODE_MAPPER_S2);

	const uint8_t links[5] = {
		m1_link,
		m2_link,
		m3_link,
		s1_link,
		s2_link };

	/* CHECK IF MIX AND IF/WHICH SPECIAL FUNCTION MUST BE CALCULATED */
	uint8_t mixUpdated = FALSE;
	uint8_t functionsUpdated[SPECIAL_FUNCITONS_NUMBER];
	for(int i = 0; i < sizeof(links); i++){		// check and update the active functions/mix
		if(links[i] / 100 == 1 && !mixUpdated){	// if the device is linked to the mix and it is not already been calculated
			FAC_mix_update();	// update the outputs values of the mix
			mixUpdated = TRUE;	// to not calculate it again
		}

		if(links[i] / 100 == 2){
			// continuare con l'update delle funzioni attive, una volta scritto il codice delle funzioni
		}
	}

	/* DC MOTOR 1*/
	if (m1_link) {	// ( m1_link == 0 -> not used) if used apply settings to it

	}
}
