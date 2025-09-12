/*
 * fac_app.h
 *
 *  Created on: Jul 31, 2025
 *      Author: filippo-castellan
 */

#ifndef INC_FAC_CODE_FAC_APP_H_
#define INC_FAC_CODE_FAC_APP_H_

#include "stm32f0xx_hal.h"

typedef struct FAC_App {
	uint8_t current_state;
	uint8_t is_low_battery;
} FAC_App;

enum FAC_STATES {
	FAC_STATE_DISARMED,
	FAC_STATE_NORMAL,
	FAC_STATE_CUTOFF,
	FAC_STATE_LAST
};

void FAC_app_init();
uint32_t map_uint32(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max);
int32_t map_int32(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max);
float map_float(float x, float in_min, float in_max, float out_min, float out_max);


#endif /* INC_FAC_CODE_FAC_APP_H_ */
