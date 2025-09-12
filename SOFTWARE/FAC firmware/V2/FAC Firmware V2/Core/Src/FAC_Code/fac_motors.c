/*
 * fac_motors.c
 *
 *  Created on: Jul 28, 2025
 *      Author: filippo-castellan
 */

#include "FAC_Code/fac_motors.h"
#include "Libraries/DMApwm.h"

#include "FAC_Code/fac_settings.h"
#include "FAC_Code/config.h"

// definitions of the 3 DC motors
static Motor motor1;
static Motor motor2;
static Motor motor3;
static Motor *motors[MOTORS_NUMBER];	// array of pointers to all the motors

/* STATIC FUNCTION PROTORYPES */
static void FAC_set_pwm_duty(uint16_t pin, uint16_t duty);
static void FAC_motor_SET_direction(uint8_t motorNumber, uint8_t dir);
static void FAC_motor_SET_reverse(uint8_t motorNumber, uint8_t isReversed);
static void FAC_motor_SET_speed(uint8_t motorNumber, uint16_t speed);
static void FAC_motor_SET_break_en(uint8_t motorNumber, uint8_t brake_en);
static void FAC_motor_apply_settings(uint8_t motorNumber);

/* FUNCTION DEFINITION */
static void FAC_set_pwm_duty(uint16_t pin, uint16_t duty) {
	setDMApwmDuty(GPIOA, pin, duty);
}

/* GET & SET FUNCITON FOR THE MOTOR PARAMETERS */
uint8_t FAC_motor_GET_direction(uint8_t motorNumber) {
	return motors[motorNumber - 1]->dir;
}

uint16_t FAC_motor_GET_speed(uint8_t motorNumber) {
	return motors[motorNumber - 1]->speed;
}

uint16_t FAC_motor_GET_reverse(uint8_t motorNumber) {
	return motors[motorNumber - 1]->is_reversed;
}

uint8_t FAC_motor_GET_brake_en(uint8_t motorNumber) {
	return motors[motorNumber - 1]->brake_en;
}

static void FAC_motor_SET_direction(uint8_t motorNumber, uint8_t dir) {
	if (motors[motorNumber - 1]->is_reversed)	// if reversed change the direction given
		motors[motorNumber - 1]->dir = !dir;
	else
		motors[motorNumber - 1]->dir = dir;
}

static void FAC_motor_SET_reverse(uint8_t motorNumber, uint8_t isReversed) {
	motors[motorNumber - 1]->is_reversed = isReversed;
}

static void FAC_motor_SET_speed(uint8_t motorNumber, uint16_t speed) {
	if (speed <= MAX_DMA_PWM_VALUE)	// (MAX_DMA_PWM_VALUE = 999, 1000-1 because zero is counted)
		motors[motorNumber - 1]->speed = speed;
	else
		motors[motorNumber - 1]->speed = MAX_DMA_PWM_VALUE;
}

static void FAC_motor_SET_break_en(uint8_t motorNumber, uint8_t brake_en) {
	motors[motorNumber - 1]->brake_en = brake_en;
}

/**
 * @brief 			Update the value of the motors. Here there is all the logic for the motor control
 * @visibility 		Visible only on this file
 * @note 			If this function is called the settings of each motor (direction and speed) are applIed
 */
static void FAC_motor_apply_settings(uint8_t motorNumber) {
	uint8_t brake_en = FAC_motor_GET_brake_en(motorNumber);
	uint8_t dir = FAC_motor_GET_direction(motorNumber);
	uint16_t speed = FAC_motor_GET_speed(motorNumber);
	uint8_t true_motor_number = motorNumber - 1;	// because the array start from zero
	/* MOTOR LOGIC (IN-IN DRIVER, NOT PH-EN) */
	/* Logic example on the motor1 M1	(use -1 on every duty because the resolution is 1000 steps, considering also zero the max number is 999 ALREADY CONSIDERED ON MAX_DMA_PWM_VALUE)
	 * 			M1_F	M1_B
	 * Forw.   1-DUTY	 1		// M1_B = 1 is with brake control enabled (more control of the motor)
	 * Reve.	 1	   1-DUTY
	 *------------------------
	 * Forw.    DUTY	 0		// M1_B = 0 is with coast control enabled (less control of the motor but no brake)
	 * Reve.	 0	    DUTY
	 *------------------------
	 * Break	 1		 1
	 * Coast	 0		 0
	 */
	if (brake_en) {	// if brake disable the logic is inverted
		if (dir == FORWARD) {
			FAC_set_pwm_duty(motors[true_motor_number]->pinB, MAX_DMA_PWM_VALUE - speed);
			FAC_set_pwm_duty(motors[true_motor_number]->pinF, MAX_DMA_PWM_VALUE);
		} else if (dir == BACKWARD) {
			FAC_set_pwm_duty(motors[true_motor_number]->pinF, MAX_DMA_PWM_VALUE - speed);
			FAC_set_pwm_duty(motors[true_motor_number]->pinB, MAX_DMA_PWM_VALUE);
		}
	} else {
		if (dir == FORWARD) {
			FAC_set_pwm_duty(motors[true_motor_number]->pinF, speed);
			FAC_set_pwm_duty(motors[true_motor_number]->pinB, 0);
		} else if (dir == BACKWARD) {
			FAC_set_pwm_duty(motors[true_motor_number]->pinB, speed);
			FAC_set_pwm_duty(motors[true_motor_number]->pinF, 0);
		}
	}

}

/* ----------------------PUBBLIC FUNCTIONS---------------------- */
/**
 * @brief 			Apply the state of the brake control of the motor
 * @visibility 		Visible everywhere
 * @note 			See motor_enable_brake and disable_brake for what brake is
 */
void FAC_motor_set_brake_status(uint8_t motorNumber, uint8_t state) {
	FAC_motor_SET_break_en(motorNumber, state);
}

/**
 * @brief 			Enable the brake control of the motor (coast control disable)
 * @visibility 		Visible everywhere
 * @note 			It make possible to control the speed more precisely, when the speed is = 0, the motors are braked (not free to spin)
 */
void FAC_motor_enable_brake(uint8_t motorNumber) {
	FAC_motor_SET_break_en(motorNumber, TRUE);
}
/**
 * @brief 			Disable the brake control of the motor (coast control enable)
 * @visibility 		Visible everywhere
 * @note 			Less precise motor speed control, but when the speed is = 0, the motors are free to spin
 */
void FAC_motor_disable_brake(uint8_t motorNumber) {
	FAC_motor_SET_break_en(motorNumber, FALSE);
}

/**
 * @brief 			Let you tell if the direction of the motor must be reversed or not
 * @visibility 		Visible everywhere
 * @note 			TRUE = Reversed, FALSE = not reversed
 */
void FAC_motor_is_reversed(uint8_t motorNumber, uint8_t isReversed) {
	FAC_motor_SET_reverse(motorNumber, isReversed);
}

/**
 * @brief 			Set the values of speed and direction of a specified motor
 * @visibility 		Visible everywhere
 * @note 			The values will directly be applied with this function
 */
void FAC_motor_set_speed_direction(uint8_t motorNumber, uint8_t dir, uint16_t speed) {
	FAC_motor_SET_direction(motorNumber, dir);	// set dir
	FAC_motor_SET_speed(motorNumber, speed);	// set speed
	FAC_motor_apply_settings(motorNumber);		// apply settings
}

/**
 * @brief 			Set the values of speed of the motor to make some beeping noise (blocking function)
 * @note 			This function block the code execution for duration ms. All DC motor will make noise
 */
void FAC_motor_make_noise(uint16_t freq, uint16_t duration) {
	FAC_DMA_pwm_change_freq(freq);
	for (int i = 1; i < MOTORS_NUMBER + 1; i++) {
		FAC_motor_SET_speed(i, 200);	// set speed at 2%
		FAC_motor_apply_settings(i);
	}
	uint32_t timerSound = HAL_GetTick();
	HAL_Delay(duration);

	while (HAL_GetTick() - timerSound <= duration) {
		for (int i = 1; i < MOTORS_NUMBER + 1; i++) {
			FAC_motor_SET_direction(i, FORWARD);
			FAC_motor_apply_settings(i);
			FAC_motor_SET_direction(i, BACKWARD);
			FAC_motor_apply_settings(i);
		}
	}
	FAC_DMA_pwm_change_freq(FAC_settings_GET_value(FAC_SETTINGS_CODE_MOTORS_FREQ));
	for (int i = 1; i < MOTORS_NUMBER + 1; i++) {
		FAC_motor_SET_speed(i, 0);	// set speed at 2%
		FAC_motor_apply_settings(i);
	}
}

/**
 * @brief 			Initialize all three motor values, and populate the array of pointers. It also initialize the DMA PWM generator
 * @visibility 		Visible everywhere
 * @note			Motors PWM are generated from the DMA
 */
void FAC_motor_Init() {
	initDMApwm(FAC_settings_GET_value(FAC_SETTINGS_CODE_MOTORS_FREQ));	// initialize the DMA PWM with the correct frequency
	/* INITIALIZE THE ARRAY OF MOTOR POINTERs */
	motors[0] = &motor1;
	motors[1] = &motor2;
	motors[2] = &motor3;

	motors[0]->pinF = M1_F_Pin;
	motors[0]->pinB = M1_B_Pin;

	motors[1]->pinF = M2_F_Pin;
	motors[1]->pinB = M2_B_Pin;

	motors[2]->pinF = M3_F_Pin;
	motors[2]->pinB = M3_B_Pin;

	for (int i = 1; i <= MOTORS_NUMBER; i++) {	// for safety reason
		FAC_motor_SET_break_en(i, TRUE);	// motor will brake_en if speed = 0
		FAC_motor_SET_direction(i, FORWARD);	// motor forward (doesn't care if speed = 0)
		FAC_motor_SET_speed(i, 0);	// motor not spinning
	}

}
