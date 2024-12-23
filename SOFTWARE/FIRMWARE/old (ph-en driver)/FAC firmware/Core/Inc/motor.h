/*
 * motor.h
 *
 *  Created on: Jul 7, 2024
 *      Author: Filippo Castellan
 */

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include "main.h"

typedef struct TankMotors {
	uint8_t speedL;		// 0-100	// 50 middle
	uint8_t speedR;		// 0-100	// 50 middle
}TankMotors;

enum MOTOR{ M1, M2, M3};

void initMotors(TIM_HandleTypeDef *htim);
void setTankMotor(TankMotors t);
void disableMotor(uint8_t motorN);
void enableMotor(uint8_t motorN);
void setMotorSpeedUnidirectional(uint8_t motorN, int8_t speed);
void setMotorSpeedBidirectional(uint8_t motorN, int8_t speed);
uint8_t calculateSpeedWithDeadZoneSingle(uint8_t speed);
uint8_t calculateSpeedWithDeadZoneDouble(uint8_t speed);
void makeSound(uint8_t motorN, uint8_t duration);

#endif /* INC_MOTOR_H_ */
