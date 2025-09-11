/*
 * config.h
 *
 *  Created on: Jul 31, 2025
 *      Author: filippo-castellan
 */

#ifndef INC_FAC_CODE_CONFIG_H_
#define INC_FAC_CODE_CONFIG_H_

/* RECEIVER */
#define RECEIVER_CHANNEL_RESOLUTION 1000	// max 2000, because the timer tick is 0.5us (500ns)
#define RECEIVER_CHANNELS_NUMBER 8

/* MOTORS */
#define MOTOR_SPEED_RESOLUTION RECEIVER_CHANNEL_RESOLUTION		// use always max resolution of the receiver

/* SERVOS */
#define SERVO_POSITION_RESOLUTION RECEIVER_CHANNEL_RESOLUTION	// use always max resolution
#define SERVO_FREQUENCY 50	// it can be selected from the settings tool (?)


#endif /* INC_FAC_CODE_CONFIG_H_ */
