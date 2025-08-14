/*
 * fac_tank_mix.c
 *
 *  Created on: Aug 11, 2025
 *      Author: filippo-castellan
 */

#include "FAC_Code/mixes/fac_tank_mix.h"
#include "FAC_Code/mixes/fac_mixes.h"

static Tankmix tankmix;

/* STATIC FUNCTION PROTORYPES */
static void FAC_tank_mix_calculate_parameters();

/* FUNCTION DEFINITION */
/* ----------------------PRIVATE FUNCTIONS---------------------- */
/*
 * @bried	Calculates motors speeds and direction (this function contains the logic of the mix)
 *
 */
static void FAC_tank_mix_calculate_parameters() {
	int16_t thValue = FAC_std_receiver_GET_channel(tankmix.throttle_channel);
	int16_t stValue = FAC_std_receiver_GET_channel(tankmix.steering_channel);
	/* calculate the speed and direction for tank mix */
	/*
	 * MR = th + st
	 * ML = th - st
	 * th [-100% , +100%] --> [-1000 , +1000]
	 * st [-100% , +100%] --> [-1000 , +1000]
	 *
	 */

	/* preparing th and st values */
	thValue = (thValue - (RECEIVER_CHANNEL_RESOLUTION - 1) / 2) * 2;	// thake the value of the thCh [0,-999] and map to [-999, +999]
	stValue = (stValue - (RECEIVER_CHANNEL_RESOLUTION - 1) / 2) * 2;	// same thing ^
	/* Apply deadzone */
	thValue = FAC_mix_calculate_dead_zone(thValue, tankmix.dead_zone_th_st, -(RECEIVER_CHANNEL_RESOLUTION-1), RECEIVER_CHANNEL_RESOLUTION-1);
	stValue = FAC_mix_calculate_dead_zone(stValue, tankmix.dead_zone_th_st, -(RECEIVER_CHANNEL_RESOLUTION-1), RECEIVER_CHANNEL_RESOLUTION-1);

	if (tankmix.throttle_reversed)
		thValue = -thValue;	// reverse the throttle if is reversed by settings
	if (tankmix.steering_reversed)
		stValue = -stValue;	// reverse the throttle if is reversed by settings
	/* the real mix */
	int16_t mr = thValue + stValue;	// mixing
	int16_t ml = thValue - stValue;	// mixing

	/* translating mr, ml to speed and direction for the motors
	 * mr = -500 means that it is going backward with 50% of the speed (assuming max speed 1000)
	 */
	/* direction */
	if (mr > 0)
		tankmix.right_motor_direction = FORWARD;
	else
		tankmix.right_motor_direction = BACKWARD;
	if (ml > 0)
		tankmix.left_motor_direction = FORWARD;
	else
		tankmix.left_motor_direction = BACKWARD;
	/* speed */
	tankmix.left_motor_speed = abs(ml);
	tankmix.right_motor_speed = abs(mr);
}

/* ----------------------PUBBLIC FUNCTIONS---------------------- */

/**
 * @biref	Update the mix calculation and the motors/servos speeds/positions
 * @note	This function directly apply the settings on the motor/servos
 */
void FAC_tank_mix_update() {
	FAC_tank_mix_calculate_parameters();	// calculate speeds and directions of motors

	/* apply motors speed and direction */
	FAC_motor_set_speed_direction(tankmix.left_motor, tankmix.left_motor_direction, tankmix.left_motor_speed);
	FAC_motor_set_speed_direction(tankmix.right_motor, tankmix.right_motor_direction, tankmix.right_motor_speed);
}

/**
 * @biref		Initialize all the Tankmix object and settings
 * @IMPORTANT	!! MUST BE CALLED IF NEW SETTINGS ARE LOADED !!
 * @param1		Deadzone for commands (th, st), expressed in percentage.
 */
void FAC_tank_mix_init(uint8_t mixChannel1Trhottle, int8_t mixChannel2Steering, uint8_t leftMotorNumber, uint8_t rightMotorNumber, uint8_t thReverse, uint8_t stReverse, uint8_t deadzone) {
	tankmix.throttle_channel = mixChannel1Trhottle;
	tankmix.steering_channel = mixChannel2Steering;
	tankmix.throttle_reversed = thReverse;
	tankmix.steering_reversed = stReverse;

	tankmix.left_motor = leftMotorNumber;
	tankmix.right_motor = rightMotorNumber;

	tankmix.dead_zone_th_st = deadzone;
}
