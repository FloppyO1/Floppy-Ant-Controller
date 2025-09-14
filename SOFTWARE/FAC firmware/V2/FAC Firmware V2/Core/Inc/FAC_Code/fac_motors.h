/*
 * fac_motors.h
 *
 *  Created on: Jul 28, 2025
 *      Author: filippo-castellan
 */

#ifndef INC_FAC_CODE_FAC_MOTORS_H_
#define INC_FAC_CODE_FAC_MOTORS_H_

#include "stm32f0xx_hal.h"
#include "main.h"

#define FORWARD TRUE
#define BACKWARD FALSE

#define MOTORS_NUMBER 3

/* TYPEDEF & STRUCT */
typedef struct Motor {
	uint8_t dir;	// direction of rotation
	uint16_t speed;	// speed of rotation
	uint8_t brake_en; // TRUE = motor will brake if speed = 0, FALSE = motor will NOT brake if speed = 0
	uint8_t is_reversed;	// TRUE/FALSE, if TRUE the motor will spin in the inverse direction of dir
	uint16_t pinF;	// corresponding hardware pin on the mcu
	uint16_t pinB;
} Motor;


/* FUNCTIONs */
void FAC_motor_set_brake_status(uint8_t motorNumber, uint8_t state);
void FAC_motor_enable_brake(uint8_t motorNumber);
void FAC_motor_disable_brake(uint8_t motorNumber);
void FAC_motor_is_reversed(uint8_t motorNumber, uint8_t isReversed);
void FAC_motor_set_speed_direction(uint8_t motorNumber, uint8_t dir, uint16_t speed);
void FAC_motor_make_noise(uint16_t freq, uint16_t duration);
void FAC_motor_init();

#endif /* INC_FAC_CODE_FAC_MOTORS_H_ */
