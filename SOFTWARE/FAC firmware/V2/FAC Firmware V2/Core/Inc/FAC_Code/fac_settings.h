/*
 * fac_settings.h
 *
 *  Created on: Aug 8, 2025
 *      Author: filippo-castellan
 */

#ifndef INC_FAC_CODE_FAC_SETTINGS_H_
#define INC_FAC_CODE_FAC_SETTINGS_H_

#include "stm32f0xx_hal.h"
#include "FAC_Code/config.h"

enum FAC_SETTINGS_CODE {
	FAC_SETTINGS_CODE_CUTOFF,
//.... ecc.
	FAC_SETTINGS_CODE_LAST	// just to have the last code possible, useful for for loop
};

typedef struct Setting {
	uint8_t code;
	uint16_t value;
	uint16_t min_value;
	uint16_t max_value;
} Setting;

void FAC_settings_USB_SEND_setting_ranges(uint8_t code);
void FAC_settings_USB_SEND_setting_value(uint8_t code);

#endif /* INC_FAC_CODE_FAC_SETTINGS_H_ */
