/*
 * fac_servo.h
 *
 *  Created on: Aug 4, 2025
 *      Author: filippo-castellan
 */

#ifndef INC_FAC_CODE_FAC_SERVO_H_
#define INC_FAC_CODE_FAC_SERVO_H_

#include "stm32f0xx_hal.h"
#include "FAC_Code/config.h"

#define SERVOS_NUMBER 2
#define MAX_SERVO_VALUE RECEIVER_CHANNEL_RESOLUTION-1

typedef struct Servo {
	uint16_t position;		// rage from 0 to max vale
	uint8_t is_enable;		// TRUE /FALSE
	uint8_t is_reversed;	// TRUE/FALSE, if TRUE the motor will spin in the inverse direction of dir
	uint16_t servo_freq;	// will be the same for every servo, it will sincronyzed on the servo set freq.
	uint16_t tim_channel;	// channel of timer used
} Servo;

uint16_t FAC_servo_GET_position(uint8_t servoNumber);
uint8_t FAC_servo_GET_is_enable(uint8_t servoNumber);
uint8_t FAC_servo_GET_is_reversed(uint8_t servoNumber);
#endif /* INC_FAC_CODE_FAC_SERVO_H_ */
