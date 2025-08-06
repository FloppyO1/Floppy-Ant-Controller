/*
 * fac_servo.c
 *
 *  Created on: Aug 4, 2025
 *      Author: filippo-castellan
 */

#include "FAC_Code/fac_servo.h"
#include "tim.h"
#include "main.h"

static Servo servo1;
static Servo servo2;
static Servo *servos[2];	// list of pointer to available servos

/* STATIC FUNCTION PROTORYPES */
static void FAC_servo_SET_position(uint8_t servoNumber, uint16_t position);
static void FAC_servo_SET_is_enable(uint8_t servoNumber, uint8_t isEnable);
static void FAC_servo_SET_is_reversed(uint8_t servoNumber, uint8_t isReversed);
static void FAC_servo_apply_new_freq();
static uint16_t FAC_servo_GET_min_ms_value(uint8_t servoNumber);
static void FAC_servo_SET_min_ms_value(uint8_t servoNumber, uint16_t minMsValue);
static uint16_t FAC_servo_GET_max_ms_value(uint8_t servoNumber);
static void FAC_servo_SET_max_ms_value(uint8_t servoNumber, uint16_t minMsValue);

/* FUNCTION DEFINITION */
/* ----------------------PRIVATE FUNCTIONS---------------------- */
static void FAC_servo_SET_position(uint8_t servoNumber, uint16_t position) {
	uint16_t p = position;
	if (p > MAX_SERVO_VALUE)	// (MAX_SERVO_VALUE = 999, 1000-1 because zero is counted)
		p = MAX_SERVO_VALUE;
	if (FAC_servo_GET_is_reversed(servoNumber))
		p = MAX_SERVO_VALUE - p;
	servos[servoNumber - 1]->position = p;
}

static void FAC_servo_SET_is_enable(uint8_t servoNumber, uint8_t isEnable) {
	servos[servoNumber - 1]->is_enable = isEnable;
}

static void FAC_servo_SET_is_reversed(uint8_t servoNumber, uint8_t isReversed) {
	servos[servoNumber - 1]->is_reversed = isReversed;
}

/**
 * @brief 	Set the servoFreq settings on all servo objects
 * @note	The servoFreq is equal for each servo used, so the value are setted for all servos
 */
static void FAC_servo_SET_freq(uint16_t servoFreq) {
	servos[0]->servo_freq = servoFreq;
	servos[1]->servo_freq = servoFreq;
}

static uint16_t FAC_servo_GET_min_ms_value(uint8_t servoNumber) {
	return servos[servoNumber]->min_ms_value;
}

static void FAC_servo_SET_min_ms_value(uint8_t servoNumber, uint16_t minMsValue) {
	servos[servoNumber]->min_ms_value = minMsValue;
}

static uint16_t FAC_servo_GET_max_ms_value(uint8_t servoNumber) {
	return servos[servoNumber]->min_ms_value;
}

static void FAC_servo_SET_max_ms_value(uint8_t servoNumber, uint16_t minMsValue) {
	servos[servoNumber]->min_ms_value = minMsValue;
}
/**
 * @brief	Set the new PWM frequency to all channels
 * @note	The PWM signal is generated with the TIM3
 * 			(fTIM: timer frequency, Tickms: tick for each ms (the resolution of servos in this case), CP: timer counter period, fs: signal frequency)
 * 			TIM Presc = fTIM/1000*Tickms	(independent from the fs)
 * 			CP = (1/fs)*1000*Tickms
 */
static void FAC_servo_apply_new_freq() {
	uint16_t fs = servos[0]->servo_freq;
	htim3.Init.Period = (1000 * SERVO_RESOLUTION) / fs;	// caluclate and set the new CP
// the tim prescaler are not recalculated, it doesn't depend from the signal frequency
	HAL_TIM_Base_Init(&htim3);	// the init must be made to update the timer CP
}

/**
 * @brief 	Update servo values
 * @note	Position already been reversed on position setter
 */
static void FAC_servo_apply_settings(uint8_t servoNumber) {
	/* set new position (new duty) if enabled, else disable pwm*/
	if (FAC_servo_GET_is_enable(servoNumber)) {
		// calculate the factored position based on the max and min values
		uint16_t p = (((servos[servoNumber - 1]->max_ms_value - servos[servoNumber - 1]->min_ms_value) / 100) * servos[servoNumber - 1]->position) / 10;
		switch (servoNumber) {
			case 1:
				// using 1000 ticks per ms the formula is the same for every frequency chosen
//				TIM3->CCR3 = 1000 + servos[servoNumber]->position;	// old method, only 1-2ms
				TIM3->CCR3 = servos[servoNumber]->min_ms_value + p;
				break;
			case 2:
				// using 1000 ticks per ms the formula is the same for every frequency chosen
//				TIM3->CCR4 = 1000 + servos[servoNumber]->position;	// old method, only 1-2ms
				TIM3->CCR4 = servos[servoNumber]->min_ms_value + p;
				break;
		}
	} else {
		switch (servoNumber) {
			case 1:
				// set to 0 to "disable any signal"
				TIM3->CCR3 = 0;
				break;
			case 2:
				//  set to 0 to "disable any signal"
				TIM3->CCR4 = 0;
				break;
		}
	}
}
/* ----------------------PUBBLIC FUNCTIONS---------------------- */
uint16_t FAC_servo_GET_position(uint8_t servoNumber) {
	return servos[servoNumber - 1]->position;
}

uint8_t FAC_servo_GET_is_enable(uint8_t servoNumber) {
	return servos[servoNumber - 1]->is_enable;
}

uint8_t FAC_servo_GET_is_reversed(uint8_t servoNumber) {
	return servos[servoNumber - 1]->is_reversed;
}

uint16_t FAC_servo_GET_servo_freq(uint8_t servoNumber) {
	return servos[servoNumber - 1]->servo_freq;
}

/**
 * @brief 	Set new servo position
 * @note 	The values will directly be applied with this function
 */
void FAC_servo_set_position(uint8_t servoNumber, uint16_t position) {
	FAC_servo_SET_position(servoNumber, position);
	FAC_servo_apply_settings(servoNumber);
}

/**
 * @brief 	Enable the servo (activate PWM)
 * @note 	Position not initialized
 */
void FAC_servo_enable(uint8_t servoNumber) {
	FAC_servo_SET_is_enable(servoNumber, TRUE);
	FAC_servo_apply_settings(servoNumber);
}

/**
 * @brief 	Disable the servo (stops PWM)
 * @note 	Position not initialized
 */
void FAC_servo_disable(uint8_t servoNumber) {
	FAC_servo_SET_is_enable(servoNumber, FALSE);
	FAC_servo_apply_settings(servoNumber);
}

/**
 * @brief 	Reverse the servo position
 * @note
 */
void FAC_servo_is_reversed(uint8_t servoNumber, uint8_t isReversed) {
	FAC_servo_SET_is_reversed(servoNumber, isReversed);
	FAC_servo_apply_settings(servoNumber);
}

/**
 * @brief 			Initialize all two servos values, and populate the array of pointers. It also initialize the TIM PWM generator
 * @note			Servos PWM are generated by TIM3 CH3/4
 */
void FAC_servo_init() {
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);

	servos[0] = &servo1;
	servos[1] = &servo2;

	for (int i = 0; i < SERVOS_NUMBER; i++) {
		servos[i]->is_enable = FALSE;
		servos[i]->is_reversed = FALSE;
		servos[i]->position = 0;
		servos[i]->servo_freq = 50;
		servos[i]->min_ms_value = 500;
		servos[i]->max_ms_value = 2500;
	}
	FAC_servo_apply_new_freq();
}

