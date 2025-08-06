/*
 * fac_app.c
 *
 *  Created on: Jul 31, 2025
 *      Author: filippo-castellan
 */

#include "FAC_Code/fac_app.h"
#include "FAC_Code/fac_adc.h"
#include "FAC_Code/fac_motors.h"
#include "FAC_Code/fac_battery.h"
#include "FAC_Code/fac_std_receiver.h"
#include "FAC_Code/fac_servo.h"

static FAC_App fac_application;

/* STATIC FUNCTION PROTORYPES */


/* FUNCTION DEFINITION */
void FAC_app_main_loop() {
	switch (fac_application.current_state) {
		case FAC_STATE_DISARMED:
			break;
		case FAC_STATE_NORMAL:
			break;
		case FAC_STATE_LIMIT:
			break;
	}
}

void FAC_app_init() {
	/* ALL INIT CODE HERE */
	FAC_adc_Init();
	FAC_motor_Init();
	FAC_battery_init();
	FAC_std_reciever_init(RECEIVER_TYPE_PWM);
	FAC_servo_init();

	fac_application.battery_voltage = 0;
	fac_application.current_state = 0;
}


/**
 * @brief 	change the range of a variable from one to another
 * @visibility everywhere
 * @retval 	return the value in the new range
 */
uint32_t map_uint32(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max) {
	if(x > in_max) x = in_max;
    // Cast to uint64_t to avoid overflow during the calculation
    return (uint32_t)(((uint64_t)(x - in_min) * (out_max - out_min)) / (in_max - in_min) + out_min);
}
