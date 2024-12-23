/*
 * motor.c
 *
 *  Created on: Jul 7, 2024
 *      Author: Filippo Castellan
 */

#include "motor.h"
#include "DMApwm.h"

// select one of the following two type of driver
#define DRIVER_IN_IN
//#define DRIVER_PH_EN

extern uint16_t speed_steps;

extern uint8_t mLeft;
extern uint8_t mRight;
extern uint8_t mWeapon;

extern uint8_t m1Rev;
extern uint8_t m2Rev;
extern uint8_t m3Rev;

void initMotors() {
	initDMApwm();
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

void setMotorSpeedBidirectional(uint8_t motorN, int16_t speed) {
	speed = calculateSpeedWithDeadZoneDouble(speed);
	speed = (speed - speed_steps / 2) * 2;
	uint16_t sp = abs(speed);
	sp = map(sp, 0, speed_steps, 0, MAX_DMA_PWM_VALUE);
	uint8_t dir;
	if (speed == 0) { // brake if speed is 0
		setDriverBrake(motorN);
		return;
	}
	if (speed >= 0)
		dir = TRUE;		// forward
	else
		dir = FALSE;	// backward

	if (getMotorRev(motorN)) dir = !dir;	//check if the motor is reversed, if it is reverse the direction

	setDriverSignals(motorN, sp, dir);
}

void setMotorSpeedUnidirectional(uint8_t motorN, int16_t speed) {
	uint16_t sp = calculateSpeedWithDeadZoneSingle(speed);
	uint8_t dir = m1Rev;
	if (speed == 0) {	// brake if speed is 0
		setDriverBrake(motorN);
		return;
	}
	if (getMotorRev(motorN)) dir = !dir;	//check if the motor is reversed, if it is reverse the direction

	setDriverSignals(motorN, sp, dir);
}

void setDriverSignals(uint8_t motorN, int16_t sp, uint8_t dir) {
#ifdef DRIVER_IN_IN
	switch (motorN) {
		case M1:
			if (dir == TRUE) {
				setDMApwmDuty(PWM_M1_B_GPIO_Port, PWM_M1_B_Pin, MAX_DMA_PWM_VALUE);
				setDMApwmDuty(PWM_M1_F_GPIO_Port, PWM_M1_F_Pin, MAX_DMA_PWM_VALUE - sp);
			} else {
				setDMApwmDuty(PWM_M1_B_GPIO_Port, PWM_M1_B_Pin, MAX_DMA_PWM_VALUE - sp);
				setDMApwmDuty(PWM_M1_F_GPIO_Port, PWM_M1_F_Pin, MAX_DMA_PWM_VALUE);
			}
			break;
		case M2:
			if (dir == TRUE) {
				setDMApwmDuty(PWM_M2_B_GPIO_Port, PWM_M2_B_Pin, MAX_DMA_PWM_VALUE);
				setDMApwmDuty(PWM_M2_F_GPIO_Port, PWM_M2_F_Pin, MAX_DMA_PWM_VALUE - sp);
			} else {
				setDMApwmDuty(PWM_M2_B_GPIO_Port, PWM_M2_B_Pin, MAX_DMA_PWM_VALUE - sp);
				setDMApwmDuty(PWM_M2_F_GPIO_Port, PWM_M2_F_Pin, MAX_DMA_PWM_VALUE);
			}
			break;
		case M3:
			if (dir == TRUE) {
				setDMApwmDuty(PWM_M3_B_GPIO_Port, PWM_M3_B_Pin, MAX_DMA_PWM_VALUE);
				setDMApwmDuty(PWM_M3_F_GPIO_Port, PWM_M3_F_Pin, MAX_DMA_PWM_VALUE - sp);
			} else {
				setDMApwmDuty(PWM_M3_B_GPIO_Port, PWM_M3_B_Pin, MAX_DMA_PWM_VALUE - sp);
				setDMApwmDuty(PWM_M3_F_GPIO_Port, PWM_M3_F_Pin, MAX_DMA_PWM_VALUE);
			}
			break;
	}
#endif

#ifdef DRIVER_PH_EN
	/*
	 * PA8 --> M1_PWM (PWM_M1_F) 	PA15 --> DIR_M1 (PWM_M1_B)
	 * PA9 --> M2_PWM (PWM_M2_F)	PA13 --> DIR_M2 (PWM_M2_B)
	 * PA10 --> M3_PWM (PWM_M3_F)	PA11 --> DIR_M3 (PWM_M3_B)
	 */
	switch (motorN) {
		case M1:
			if (dir) {	// set the direction
				setDMApwmDuty(PWM_M1_B_GPIO_Port, PWM_M1_B_Pin, MAX_DMA_PWM_VALUE);
			} else {
				setDMApwmDuty(PWM_M1_B_GPIO_Port, PWM_M1_B_Pin, 0);
			}
			setDMApwmDuty(PWM_M1_F_GPIO_Port, PWM_M1_F_Pin, sp);	// set the speed
			break;
		case M2:
			if (dir) {	// set the direction
				setDMApwmDuty(PWM_M2_B_GPIO_Port, PWM_M2_B_Pin, MAX_DMA_PWM_VALUE);
			} else {
				setDMApwmDuty(PWM_M2_B_GPIO_Port, PWM_M2_B_Pin, 0);
			}
			setDMApwmDuty(PWM_M2_F_GPIO_Port, PWM_M2_F_Pin, sp);	// set the speed
			break;
		case M3:
			if (dir) {	// set the direction
				setDMApwmDuty(PWM_M2_B_GPIO_Port, PWM_M3_B_Pin, MAX_DMA_PWM_VALUE);
			} else {
				setDMApwmDuty(PWM_M3_B_GPIO_Port, PWM_M3_B_Pin, 0);
			}
			setDMApwmDuty(PWM_M3_F_GPIO_Port, PWM_M3_F_Pin, sp);	// set the speed
			break;
	}
#endif
}

void setDriverBrake(uint8_t motorN) {
#ifdef DRIVER_IN_IN
	switch (motorN) {
		case M1:
			setDMApwmDuty(PWM_M1_B_GPIO_Port, PWM_M1_B_Pin, MAX_DMA_PWM_VALUE);
			setDMApwmDuty(PWM_M1_F_GPIO_Port, PWM_M1_F_Pin, MAX_DMA_PWM_VALUE);
			break;
		case M2:
			setDMApwmDuty(PWM_M2_B_GPIO_Port, PWM_M2_B_Pin, MAX_DMA_PWM_VALUE);
			setDMApwmDuty(PWM_M2_F_GPIO_Port, PWM_M2_F_Pin, MAX_DMA_PWM_VALUE);
			break;
		case M3:
			setDMApwmDuty(PWM_M3_B_GPIO_Port, PWM_M3_B_Pin, MAX_DMA_PWM_VALUE);
			setDMApwmDuty(PWM_M3_F_GPIO_Port, PWM_M3_F_Pin, MAX_DMA_PWM_VALUE);
			break;
	}
#endif

#ifdef DRIVER_PH_EN
	switch (motorN) {
		case M1:
			setDMApwmDuty(PWM_M1_F_GPIO_Port, PWM_M1_F_Pin, 0);
			break;
		case M2:
			setDMApwmDuty(PWM_M2_F_GPIO_Port, PWM_M2_F_Pin, 0);
			break;
		case M3:
			setDMApwmDuty(PWM_M3_F_GPIO_Port, PWM_M3_F_Pin, 0);
			break;
	}
#endif
}

/*	Calculate the speed with dead-zone for single direction DC motor and servos
 *  Center +-3, max -3, min +3
 *	change the deadzone value according your needs
 */
#define DEADZONE 10
int16_t calculateSpeedWithDeadZoneSingle(int16_t speed) {
	uint16_t returnValue = speed;
	if (speed > (speed_steps / 2 - DEADZONE) && speed < (speed_steps / 2 + DEADZONE)) returnValue = speed_steps / 2;// central deadzone
	if (speed < DEADZONE) returnValue = 0;	// lower deadzone
	if (speed > speed_steps - DEADZONE) returnValue = speed_steps;	// upper deadzone
	return returnValue;
}
/*	Calculate the speed with dead-zone for double direction DC motors
 *  max -3, min +3
 *	change the deadzone value according your needs
 */
int16_t calculateSpeedWithDeadZoneDouble(int16_t speed) {
	int16_t returnValue = speed;
	if (speed < DEADZONE) returnValue = 0;	// lower deadzone
	if (speed > speed_steps - DEADZONE) returnValue = speed_steps;	// upper deadzone
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
	uint16_t sp = (MAX_DMA_PWM_VALUE / 100) * 2;
	while (HAL_GetTick() <= time + duration) {
#ifdef DRIVER_IN_IN
		switch (motorN) {
			case M1:
				setDMApwmDuty(PWM_M1_B_GPIO_Port, PWM_M1_B_Pin, MAX_DMA_PWM_VALUE);
				setDMApwmDuty(PWM_M1_F_GPIO_Port, PWM_M1_F_Pin, MAX_DMA_PWM_VALUE - sp);
				break;
			case M2:
				setDMApwmDuty(PWM_M2_B_GPIO_Port, PWM_M2_B_Pin, MAX_DMA_PWM_VALUE);
				setDMApwmDuty(PWM_M2_F_GPIO_Port, PWM_M2_F_Pin, MAX_DMA_PWM_VALUE - sp);
				break;
			case M3:
				setDMApwmDuty(PWM_M3_B_GPIO_Port, PWM_M3_B_Pin, MAX_DMA_PWM_VALUE);
				setDMApwmDuty(PWM_M3_F_GPIO_Port, PWM_M3_F_Pin, MAX_DMA_PWM_VALUE - sp);
				break;
		}
#endif

#ifdef DRIVER_PH_EN
		switch (motorN) {
			case M1:
				setDMApwmDuty(PWM_M1_B_GPIO_Port, PWM_M1_B_Pin, 1);
				setDMApwmDuty(PWM_M1_F_GPIO_Port, PWM_M1_F_Pin, MAX_DMA_PWM_VALUE - sp);
				break;
			case M2:
				setDMApwmDuty(PWM_M2_B_GPIO_Port, PWM_M2_B_Pin, 1);
				setDMApwmDuty(PWM_M2_F_GPIO_Port, PWM_M2_F_Pin, MAX_DMA_PWM_VALUE - sp);
				break;
			case M3:
				setDMApwmDuty(PWM_M3_B_GPIO_Port, PWM_M3_B_Pin, 1);
				setDMApwmDuty(PWM_M3_F_GPIO_Port, PWM_M3_F_Pin, MAX_DMA_PWM_VALUE - sp);
				break;
		}
#endif
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
