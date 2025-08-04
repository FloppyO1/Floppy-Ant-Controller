/*
 * fac_pwm_receiver.c
 *
 *	To calculate the time, an interrupt is used that takes the
 *	value of a 32 bit timer (TIM2) that can count up to 35 minutes
 *	with a tick every 1/2microsecond. It takes the difference between
 *	the time of the rising edge and the falling edge to obtain the
 *	number of microseconds of the pulse.
 *
 *  Created on: Aug 3, 2025
 *      Author: filippo-castellan
 */

#include "FAC_Code/fac_pwm_receiver.h"
#include "FAC_Code/fac_std_receiver.h"
#include "FAC_Code/fac_app.h"
#include "main.h"
#include "tim.h"

static Pwm_receiver pwmReceiver;

/* STATIC FUNCTION PROTORYPES */

/* FUNCTION DEFINITION */

/**
 * @brief		This function store the current time in t1 or t2 in base at witch edge is detected from the exti
 * @visibility	Everywhere
 * @param1		edge: edge detected, 0 falling correspond to GPIO_PIN_RESET, 1 rising correspond to GPIO_PIN_SET
 * @param2		GPIO_Pin: which pin triggered the interrupt
 */
void FAC_pwm_receiver_Callback(uint8_t edge, uint16_t GPIO_Pin) {
	uint32_t t = __HAL_TIM_GET_COUNTER(&htim2);
	if (edge == RISING) {
		switch (GPIO_Pin) {
			case CH1_Pin:
				pwmReceiver.channels_t1[0] = t;
				break;
			case CH2_Pin:
				pwmReceiver.channels_t1[1] = t;
				break;
			case CH3_Pin:
				pwmReceiver.channels_t1[2] = t;
				break;
			case CH4_Pin:
				pwmReceiver.channels_t1[3] = t;
				break;
		}
	} else {	// if edge == FALLING
		switch (GPIO_Pin) {
			case CH1_Pin:
				pwmReceiver.channels_t2[0] = t;
				break;
			case CH2_Pin:
				pwmReceiver.channels_t2[1] = t;
				break;
			case CH3_Pin:
				pwmReceiver.channels_t2[2] = t;
				break;
			case CH4_Pin:
				pwmReceiver.channels_t2[3] = t;
				break;
		}
	}
}

/**
 * @brief		Calculate the correct channel value from 0 to RECEIVER_CHANNEL_RESOLUTION. It will send it to the std_receiver object
 * @visibility	everyweher
 * @retval		the value of the channel
 * @note		TIM2 has a resolution of 0.5us (~35 min of period), not used the FAC_std_receiver_new_channel_value in rage return value
 */
void FAC_pwm_receiver_calculate_channel_value(uint8_t chNumber) {
		uint32_t t1 = pwmReceiver.channels_t1[chNumber - 1];
		uint32_t t2 = pwmReceiver.channels_t2[chNumber - 1];
	if (t2 > t1) {	// the received value must be valid
		uint16_t value = (uint16_t) (t2-t1);
		if (value <= MAX_TIM2_TEORETICAL_CHANNEL_COUNT + (MAX_TIM2_TEORETICAL_CHANNEL_COUNT / 100) * 10) {// if the count is grater than a 10% the value is ignored it must be something wrong with the time capture
			if (value < MAX_TIM2_TEORETICAL_CHANNEL_COUNT / 2)
				value = MAX_TIM2_TEORETICAL_CHANNEL_COUNT / 2;// sometime the transmitter has a grater range than the expected, so the min value is set to 1000 to not have any problem on the calculus
			value = map_uint32(value - (MAX_TIM2_TEORETICAL_CHANNEL_COUNT / 2), 0, MAX_TIM2_TEORETICAL_CHANNEL_COUNT/2, 0, RECEIVER_CHANNEL_RESOLUTION - 1);
			// if(value > RECEIVER_CHANNEL_RESOLUTION-1) value = RECEIVER_CHANNEL_RESOLUTION-1;	// not needed the value is already cropped on the map_uint32_t function
			FAC_std_receiver_new_channel_value(chNumber, value);
		}
	}
	/* Reset the values of t1 and t2
	 * If this is not done the value can never be zero
	 */
	pwmReceiver.channels_t2[chNumber - 1] = 0;
	pwmReceiver.channels_t1[chNumber - 1] = 0;
}

/**
 * @brief Initialize the pwmReceiver object and starts the tim2
 *
 */
void FAC_pwm_receiver_init() {
	HAL_TIM_Base_Start(&htim2);

	for (int i = 0; i < PWM_RECEIVER_CHANNELS_NUMBER; i++) {
		pwmReceiver.channels_t1[i] = 0;
		pwmReceiver.channels_t2[i] = 0;
	}
}

