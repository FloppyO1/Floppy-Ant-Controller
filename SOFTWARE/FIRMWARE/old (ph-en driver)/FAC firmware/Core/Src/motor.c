/*
 * motor.c
 *
 *  Created on: Jul 7, 2024
 *      Author: Filippo Castellan
 */

#include "motor.h"

extern uint8_t mLeft;
extern uint8_t mRight;
extern uint8_t mWeapon;

extern uint8_t m1Rev;
extern uint8_t m2Rev;
extern uint8_t m3Rev;

void initMotors(TIM_HandleTypeDef *htim) {
	HAL_TIM_PWM_Init(htim);
	HAL_TIM_PWM_Start(htim, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(htim, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(htim, TIM_CHANNEL_3);
}

void setTankMotor(TankMotors t) {
	// set the speed of the motors
	setMotorSpeedBidirectional(mLeft, t.speedL);
	setMotorSpeedBidirectional(mRight, t.speedR);
}

uint8_t getMotorRev(uint8_t motorN) {	// return 1 if the motorN is reversed, else 0
	uint8_t isRev = 0;
	switch (motorN) {
		case M1:
			isRev = m1Rev;
			break;
		case M2:
			isRev = m2Rev;
			break;
		case M3:
			isRev = m3Rev;
			break;
	}
	return isRev;
}

void setMotorSpeedBidirectional(uint8_t motorN, int8_t speed) {
	speed = calculateSpeedWithDeadZoneDouble(speed);
	speed = (speed - 50) * 2;
	uint8_t sp = abs(speed);
	uint8_t dir;
	if (speed >= 0)
		dir = TRUE;
	else
		dir = FALSE;

	if (getMotorRev(motorN)) dir = !dir;	//check if the motor is reversed, if it is reverse the direction

	switch (motorN) {
		case M1:
			HAL_GPIO_WritePin(DIR_M1_GPIO_Port, DIR_M1_Pin, dir);
			TIM1->CCR1 = sp;
			break;
		case M2:
			HAL_GPIO_WritePin(DIR_M2_GPIO_Port, DIR_M2_Pin, dir);
			TIM1->CCR2 = sp;
			break;
		case M3:
			HAL_GPIO_WritePin(DIR_M3_GPIO_Port, DIR_M3_Pin, dir);
			TIM1->CCR3 = sp;
			break;
	}
}

void setMotorSpeedUnidirectional(uint8_t motorN, int8_t speed) {
	uint8_t sp = calculateSpeedWithDeadZoneSingle(speed);
	uint8_t dir = m1Rev;

	if (getMotorRev(motorN)) dir = !dir;	//check if the motor is reversed, if it is reverse the direction

	switch (motorN) {
		case M1:
			HAL_GPIO_WritePin(DIR_M1_GPIO_Port, DIR_M1_Pin, dir);
			TIM1->CCR1 = sp;
			break;
		case M2:
			HAL_GPIO_WritePin(DIR_M2_GPIO_Port, DIR_M2_Pin, dir);
			TIM1->CCR2 = sp;
			break;
		case M3:
			HAL_GPIO_WritePin(DIR_M3_GPIO_Port, DIR_M3_Pin, dir);
			TIM1->CCR3 = sp;
			break;
	}
}

/*	Calculate the speed with dead-zone for single direction DC motor and servos
 *  Center +-3, max -3, min +3
 *	change the deadzone value according your needs
 */
#define DEADZONE 10
uint8_t calculateSpeedWithDeadZoneSingle(uint8_t speed) {
	uint8_t returnValue = speed;
	if (speed > (50 - DEADZONE) && speed < (50 + DEADZONE)) returnValue = 50;	// central deadzone
	if (speed < DEADZONE) returnValue = 0;	// lower deadzone
	if (speed > 100 - DEADZONE) returnValue = 100;	// upper deadzone
	return returnValue;
}
/*	Calculate the speed with dead-zone for double direction DC motors
 *  max -3, min +3
 *	change the deadzone value according your needs
 */
uint8_t calculateSpeedWithDeadZoneDouble(uint8_t speed) {
	uint8_t returnValue = speed;
	if (speed < DEADZONE) returnValue = 0;	// lower deadzone
	if (speed > 100 - DEADZONE) returnValue = 100;	// upper deadzone
	return returnValue;
}

/*	This is a blocking function that causes the motors to beep,
 *  keeping the duration as short as possible.
 * @duration:
 *
 */
void makeSound(uint8_t motorN, uint8_t duration) {
	uint32_t time = HAL_GetTick();
	enableMotor(motorN);
	while (HAL_GetTick() <= time + duration) {
		switch (motorN) {
			case M1:
				TIM1->CCR1 = 2;
				break;
			case M2:
				TIM1->CCR2 = 2;
				break;
			case M3:
				TIM1->CCR3 = 2;
				break;
		}
	}
	disableMotor(motorN);
}

void disableMotor(uint8_t motorN) {
	switch (motorN) {
		case M1:
			HAL_GPIO_WritePin(SLEEPN_M1_GPIO_Port, SLEEPN_M1_Pin, FALSE);
			break;
		case M2:
			HAL_GPIO_WritePin(SLEEPN_M2_GPIO_Port, SLEEPN_M2_Pin, FALSE);
			break;
		case M3:
			HAL_GPIO_WritePin(SLEEPN_M3_GPIO_Port, SLEEPN_M3_Pin, FALSE);
			break;
	}
}

void enableMotor(uint8_t motorN) {
	switch (motorN) {
		case M1:
			HAL_GPIO_WritePin(SLEEPN_M1_GPIO_Port, SLEEPN_M1_Pin, TRUE);
			break;
		case M2:
			HAL_GPIO_WritePin(SLEEPN_M2_GPIO_Port, SLEEPN_M2_Pin, TRUE);
			break;
		case M3:
			HAL_GPIO_WritePin(SLEEPN_M3_GPIO_Port, SLEEPN_M3_Pin, TRUE);
			break;
	}
}
