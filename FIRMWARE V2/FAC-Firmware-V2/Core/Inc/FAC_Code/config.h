/*
 * config.h
 *
 *  Created on: Jul 31, 2025
 *      Author: filippo-castellan
 */

#ifndef INC_FAC_CODE_CONFIG_H_
#define INC_FAC_CODE_CONFIG_H_

#ifdef DEBUG
//#define IM_TESTING_FAC_TOOL			// if this is defined the target use only the mcu and eeprom, used for testing settings and nothing else.
//#define SERIAL_DEBUG	// activate the serial debug prints
#endif

/* FIRMWARE VERSION */
// VERSION NOTE (in addition to the previews one):
// Now the FAC will always boot up and is forced to DISARMED state until a valid signal came from the receiver
// than the arming channel/setting will decide the state
#define FIRMWARE_VERSION_MAJOR 2	// [MAX 9] 	add one here, if the changes are substantial and not backwards compatible
#define FIRMWARE_VERSION_MINOR 0	// [MAX 99] add one here, if new backwards compatible features have been added
#define FIRMWARE_VERSION_PATCH 7	// [MAX 99] add one here, if any issues have been fixed without adding any features

#define FIRMWARE_VERSION_TAG  (uint8_t)((FIRMWARE_VERSION_MAJOR * 101U + FIRMWARE_VERSION_MINOR * 7U + FIRMWARE_VERSION_PATCH * 3U) % 255U)



/* RECEIVER */
#define RECEIVER_CHANNEL_RESOLUTION 1000	// max 2000, because the timer tick is 0.5us (500ns)
#define RECEIVER_CHANNELS_NUMBER 8

/* MOTORS */
#define MOTOR_SPEED_RESOLUTION RECEIVER_CHANNEL_RESOLUTION		// use always max resolution of the receiver

/* SERVOS */
#define SERVO_POSITION_RESOLUTION RECEIVER_CHANNEL_RESOLUTION	// use always max resolution
#define SERVO_FREQUENCY 50	// it can be selected from the settings tool (?)

/* FAC APP */
#define ARMING_THRESHOLD 80				// 80% of the max receiver number

#define DISARMED_LED_PERIOD	1000			// in milliseconds
#define DISARMED_TONE_PERIOD 500
#define DISARMED_TONE_DURATION 100

#define LOW_BATTERY_LED_PERIOD 500

#define CUT_OFF_LED_PERIOD	200
#define CUT_OFF_TONE_PERIOD 300
#define CUT_OFFTONE_DURATION 100

//#define NORMAL_LED_PERIOD	3000		// in milliseconds

#endif /* INC_FAC_CODE_CONFIG_H_ */
