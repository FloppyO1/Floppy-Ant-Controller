/*
 * motor.h
 *
 *  Created on: Jul 7, 2024
 *      Author: Filippo Castellan
 */

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include "main.h"
#include "DMApwm.h"

typedef struct TankMotors {
	int16_t speedL;		// 0-100%	// 50% middle
	int16_t speedR;		// 0-100%	// 50% middle
}TankMotors;

enum MOTOR{ M1, M2, M3};

void initMotors();
void setTankMotor(TankMotors t);
void disableMotor(uint8_t motorN);
void enableMotor(uint8_t motorN);
void setMotorSpeedUnidirectional(uint8_t motorN, int16_t speed);
void setMotorSpeedBidirectional(uint8_t motorN, int16_t speed);
void setDriverBrake(uint8_t motorN);
void setDriverSignals(uint8_t motorN, int16_t sp, uint8_t dir);
int16_t calculateSpeedWithDeadZoneSingle(int16_t speed);
int16_t calculateSpeedWithDeadZoneDouble(int16_t speed);
void makeSound(uint8_t motorN, uint8_t duration);

#endif /* INC_MOTOR_H_ */
