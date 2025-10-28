/*
 * fac_app.c
 *
 *  Created on: Jul 31, 2025
 *      Author: filippo-castellan
 */
#include "FAC_Code/fac_app.h"
#include "FAC_Code/config.h"

#include "FAC_Code/mixes_functions/fac_mixes.h"
#include "FAC_Code/mixes_functions/fac_functions.h"
#include "FAC_Code/fac_mapper.h"
#include "FAC_Code/fac_adc.h"
#include "FAC_Code/fac_motors.h"
#include "FAC_Code/fac_battery.h"
#include "FAC_Code/fac_std_receiver.h"
#include "FAC_Code/fac_servo.h"
#include "FAC_Code/fac_settings.h"
#include "jingles/fac_jingles.h"
#include "FAC_Code/fac_imu.h"

static FAC_App fac_application;
uint8_t newComSerialReceived = FALSE;	// turn true when something is received

/* STATIC FUNCTION PROTORYPES */

/* STATIC FUNCTION */
static void FAC_app_SET_current_state(uint8_t current_state) {
	if (current_state < FAC_STATE_LAST)
		fac_application.current_state = current_state;
}

uint8_t FAC_app_GET_current_state() {
	return fac_application.current_state;
}

static void FAC_app_SET_is_low_battery(uint8_t is_low_battery) {
	fac_application.is_low_battery = is_low_battery;
}

static uint8_t FAC_app_GET_is_low_battery() {
	return fac_application.is_low_battery;
}

/* FUNCTION DEFINITION */
void FAC_app_main_loop() {	// one cycle every 13ms [about 76Hz] (with simple tank mix on and two other direct link function)
//	HAL_GPIO_TogglePin(DIGITAL_AUX1_GPIO_Port, DIGITAL_AUX1_Pin);	// used to see the time of execution
	if (newComSerialReceived) {		//	1us
		// understand the command received and do what you have to do
		FAC_settings_command_response();

		newComSerialReceived = FALSE;
	}

	/* MAIN FUNCTIONS OF THE APP - STATES OF OPERATION */	// 13ms
	switch (FAC_app_GET_current_state()) {
		case FAC_STATE_DISARMED: {
			/* DISABLE ALL DEVICES (MOTORS AND SERVOS) */
			for (int i = 1; i < MOTORS_NUMBER + 1; i++) {
				FAC_motor_set_speed_direction(i, FORWARD, 0);
			}
			for (int i = 1; i < SERVOS_NUMBER + 1; i++) {
				FAC_servo_disable(i);
			}

			/* CHECK ARMING CHANNEL IF USED */
			uint8_t armingCh = FAC_settings_GET_value(FAC_SETTINGS_CODE_ARMING_CHANNEL);
			if (armingCh != 0) {
				uint16_t armingValue = FAC_std_receiver_GET_channel(armingCh);
				if (armingValue >= (RECEIVER_CHANNEL_RESOLUTION / 100) * ARMING_THRESHOLD) {	// value above the threschold
					FAC_app_SET_current_state(FAC_STATE_NORMAL);
				}
			}else{
				FAC_app_SET_current_state(FAC_STATE_NORMAL);	// always armed if no arming channel active
			}

			/* DISARMED TONE */
			static uint32_t timerTone = 0;
			if (HAL_GetTick() - timerTone > DISARMED_TONE_PERIOD / 2) {
				timerTone = HAL_GetTick();
				FAC_motor_make_noise(750, DISARMED_TONE_DURATION);
			}

			/* STATUS LED BLINK */
			static uint32_t timerStatusLed = 0;
			if (HAL_GetTick() - timerStatusLed > DISARMED_LED_PERIOD / 2) {
				timerStatusLed = HAL_GetTick();
				HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
			}

			break;
		}
		case FAC_STATE_NORMAL: {
			/* UPDATE THE MAPPED MOTORS AND SERVOS */   // 8ms max
			FAC_mapper_apply_to_devices();

			/* CHECK ARMING CHANNEL IF USED */	// 800us max
			uint8_t armingCh = FAC_settings_GET_value(FAC_SETTINGS_CODE_ARMING_CHANNEL);
			if (armingCh != 0) {
				uint16_t armingValue = FAC_std_receiver_GET_channel(armingCh);
				if (armingValue <= (RECEIVER_CHANNEL_RESOLUTION / 100) * ARMING_THRESHOLD) {	// value above the threschold
					FAC_app_SET_current_state(FAC_STATE_DISARMED);
				}
			}


			/* LOW BATTERY DETECTOR */		// 200us
			uint16_t vbat = FAC_battery_GET_cell_voltage();
			uint8_t batteryType = FAC_battery_GET_type();
			if (batteryType != BATTERY_TYPE_NONE && batteryType != BATTERY_TYPE_USB) {	// a known battery must be connected
				if (vbat < FAC_settings_GET_value(FAC_SETTINGS_CODE_LOW_BATTERY_VOLTAGE_MV))	// if the vbat is below the low battery thershold
					FAC_app_SET_is_low_battery(TRUE);
			}

			/* CUT OFF DETECTION */		// 8us
			static uint32_t timerCutOff = 0;
			if (vbat > FAC_settings_GET_value(FAC_SETTINGS_CODE_CUTOFF_VOLTAGE_MV))
				timerCutOff = HAL_GetTick();	// if the vbat is grater than the cutoff threshold the timer will be resetted
			if (HAL_GetTick() - timerCutOff > FAC_settings_GET_value(FAC_SETTINGS_CODE_CUTOFF_DETECTION_TIME)) {
				FAC_app_SET_current_state(FAC_STATE_CUTOFF);
			}

			/* STATUS LED */
			static uint32_t timerLowBattery = 0;
			if (FAC_app_GET_is_low_battery()) {
				if (HAL_GetTick() - timerLowBattery > LOW_BATTERY_LED_PERIOD / 2) {
					timerLowBattery = HAL_GetTick();
					HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
				}
			} else {
				HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, 1);	// if no low battery detected the led will be always on
			}

			break;
		}
		case FAC_STATE_CUTOFF: {
			/* STATUS LED BLINK */
			static uint32_t timerStatusLed = 0;
			if (HAL_GetTick() - timerStatusLed > CUT_OFF_LED_PERIOD / 2) {
				timerStatusLed = HAL_GetTick();
				HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
			}
			/* CUT-OFF TONE */
			static uint32_t timerTone = 0;
			if (HAL_GetTick() - timerTone > CUT_OFF_TONE_PERIOD / 2) {
				timerTone = HAL_GetTick();
				FAC_motor_make_noise(750, CUT_OFFTONE_DURATION);
			}
			break;
		}
	}

	/* ONE SECOND FUNCTION */
	static uint32_t time = 0;
	if (HAL_GetTick() - time >= 1000) {
		time = HAL_GetTick();
		/* WRITE HERE YOUR CODE */

	}
}

/*
 * @brief	Initialize all modules and load from eeprom all the settings
 *
 */
void FAC_app_init() {
	FAC_settings_init(1);	/// first load all settings than initialize all modules

	FAC_adc_Init();
	FAC_battery_init();
	/* INERTIAL MESUREMENT UNIT INIT */
	HAL_Delay(3000);
	FAC_IMU_init();
	FAC_IMU_init_accelerometer();
	FAC_IMU_init_gyroscope();
	if (FAC_IMU_GET_status() == HAL_ERROR) {
		for (int i = 0; i < 20; i++) {
			HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
			HAL_Delay(100);
		}
	} else {
		FAC_IMU_compute_gyro_offset();
	}

	/* INIT SETTING RELATED MODULES */
	FAC_app_init_all_modules();

	/* INITIALIZE APP STRUCT */
	fac_application.is_low_battery = FALSE;
	FAC_app_SET_current_state(FAC_STATE_DISARMED);

	/* INIT END */
//	FAC_jingle_Tequila_long();
	FAC_jingle_neverGiveYouUp();

}

/*
 * @brief	Initialize all modules dependent to setting values
 * @note	EEPROM is not initialized
 *
 */
void FAC_app_init_all_modules() {
	FAC_motor_init();
	FAC_std_reciever_init(FAC_settings_GET_value(FAC_SETTINGS_CODE_RECEIVER_TYPE));	// must be changed in base of settings
	FAC_servo_init();
	FAC_mixes_init();
	FAC_functions_init();
}

/**
 * @brief 		change the range of a variable from one to another
 * @retval 		return the value in the new range
 */
uint32_t map_uint32(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max) {
	if (x > in_max)
		x = in_max;
	// Cast to uint64_t to avoid overflow during the calculation
	return (uint32_t) (((uint64_t) (x - in_min) * (out_max - out_min)) / (in_max - in_min) + out_min);
}

/**
 * @brief 		change the range of a variable from one to another
 * @retval 		return the value in the new range
 */
int32_t map_int32(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max) {
	if (x > in_max)
		x = in_max;
	// Cast to uint64_t to avoid overflow during the calculation
	return (int32_t) (((int64_t) (x - in_min) * (out_max - out_min)) / (in_max - in_min) + out_min);
}

/**
 * @brief 		change the range of a variable from one to another
 * @retval 		return the value in the new range
 */
float map_float(float x, float in_min, float in_max, float out_min, float out_max) {
	// Limita il valore di input 'x' per assicurarti che rimanga all'interno del suo intervallo
	if (x > in_max)
		x = in_max;

	if (x < in_min)
		x = in_min;

	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
