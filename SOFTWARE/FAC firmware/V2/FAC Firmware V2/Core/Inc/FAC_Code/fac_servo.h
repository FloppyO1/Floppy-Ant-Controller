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

typedef struct Servo {
	uint16_t position;		// rage from 0 to max vale
	uint8_t is_enable;		// TRUE /FALSE
	uint8_t is_reversed;	// TRUE/FALSE, if TRUE the motor will spin in the inverse direction of dir
} Servo;

#endif /* INC_FAC_CODE_FAC_SERVO_H_ */
