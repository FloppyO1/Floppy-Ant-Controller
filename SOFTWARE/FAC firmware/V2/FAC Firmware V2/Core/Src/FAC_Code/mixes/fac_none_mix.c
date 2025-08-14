/*
 * fac_none_mix.c
 *
 *  Created on: Aug 14, 2025
 *      Author: filippo-castellan
 */

#include "FAC_Code/mixes/fac_none_mix.h"

static Nonemix noneMix;

/* STATIC FUNCTION PROTORYPES */
static void FAC_none_mix_SET_dead_zone(uint8_t deadzone);

/* FUNCTION DEFINITION */
/* ----------------------PRIVATE FUNCTIONS---------------------- */

static void FAC_none_mix_SET_dead_zone(uint8_t deadzone) {
	noneMix.dead_zone = deadzone;
}

/* ----------------------PUBBLIC FUNCTIONS---------------------- */
uint8_t FAC_none_mix_GET_dead_zone() {
	return noneMix.dead_zone;
}

/**
 * @biref	Update the motors/servos speeds/positions
 * @note	This function directly apply the settings on the motor/servos
 */
void FAC_none_mix_update() {
	/* get receiver values */
	int16_t channels[6];
	for (int i = 1; i < 6; i++) {
		channels[i] = FAC_std_receiver_GET_channel(i);
	}
//	int16_t ch1 = FAC_std_receiver_GET_channel(1);	// motor double direction
//	int16_t ch2 = FAC_std_receiver_GET_channel(2);	// motor double direction
//	int16_t ch3 = FAC_std_receiver_GET_channel(3);	// servo
//	int16_t ch4 = FAC_std_receiver_GET_channel(4);	// servo
//	int16_t ch5 = FAC_std_receiver_GET_channel(5);	// motor double direction

	/* center the value for motors */
	channels[1] = (channels[1] - (RECEIVER_CHANNEL_RESOLUTION - 1) / 2) * 2;
	channels[2] = (channels[2] - (RECEIVER_CHANNEL_RESOLUTION - 1) / 2) * 2;
	channels[5] = (channels[5] - (RECEIVER_CHANNEL_RESOLUTION - 1) / 2) * 2;
	/* Apply deadzone */
	channels[1] = FAC_mix_calculate_dead_zone(channels[1], noneMix.dead_zone, -(RECEIVER_CHANNEL_RESOLUTION - 1), RECEIVER_CHANNEL_RESOLUTION - 1);
	channels[2] = FAC_mix_calculate_dead_zone(channels[2], noneMix.dead_zone, -(RECEIVER_CHANNEL_RESOLUTION - 1), RECEIVER_CHANNEL_RESOLUTION - 1);
	channels[5] = FAC_mix_calculate_dead_zone(channels[5], noneMix.dead_zone, -(RECEIVER_CHANNEL_RESOLUTION - 1), RECEIVER_CHANNEL_RESOLUTION - 1);

	channels[3] = FAC_mix_calculate_dead_zone(channels[3], noneMix.dead_zone, 0, RECEIVER_CHANNEL_RESOLUTION - 1);
	channels[4] = FAC_mix_calculate_dead_zone(channels[4], noneMix.dead_zone, 0, RECEIVER_CHANNEL_RESOLUTION - 1);

	/* calculate direction ad speed of motors */
	/* translating mr, ml to speed and direction for the motors
	 * mr = -500 means that it is going backward with 50% of the speed (assuming max speed 1000)
	 */
	/* direction */
	uint8_t m1dir;
	uint8_t m2dir;
	uint8_t m3dir;
	for (int i = 1; i < 6; i++) {
		switch (i) {
			case 1:
				if (channels[i] > 0)
					m1dir = FORWARD;
				else
					m1dir = BACKWARD;
				break;
			case 2:
				if (channels[i] > 0)
					m2dir = FORWARD;
				else
					m2dir = BACKWARD;
				break;
			case 5:
				if (channels[i] > 0)
					m3dir = FORWARD;
				else
					m3dir = BACKWARD;
				break;
		}
	}
	/* Apply value to motors and servos */
	FAC_motor_set_speed_direction(noneMix.channel1_motor_number, m1dir, abs(channels[1]));
	FAC_motor_set_speed_direction(noneMix.channel2_motor_number, m2dir, abs(channels[2]));
	FAC_motor_set_speed_direction(noneMix.channel5_motor_number, m3dir, abs(channels[5]));

	FAC_servo_set_position(noneMix.channel3_servo_number, channels[3]);
	FAC_servo_set_position(noneMix.channel4_servo_number, channels[4]);
}

/*
 * @brief	The initialization used when no mixes are active
 * @note	Channels are connected as below (all motors have reverse):
 * 			Ch1 ->	Motor1
 * 			Ch2 ->	Motor2
 * 			Ch3 ->	Servo1
 * 			Ch4 ->	Servo2
 * 			Ch5 ->	Motor3
 * 			Ch6 ->	Arming function (active low = 0)
 * 			Ch7 ->	none
 * 			Ch8 ->	none
 */
void FAC_none_mix_init(uint8_t deadzone) {
	FAC_none_mix_SET_dead_zone(deadzone);
	noneMix.channel1_motor_number = 1;
	noneMix.channel2_motor_number = 2;
	noneMix.channel5_motor_number = 3;

	noneMix.channel3_servo_number = 1;
	noneMix.channel4_servo_number = 2;

}
