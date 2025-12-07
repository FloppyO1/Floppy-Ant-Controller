/*
 * fac_ppm_receiver.c
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

#include "FAC_Code/fac_ppm_receiver.h"
#include "FAC_Code/fac_std_receiver.h"
#include "FAC_Code/fac_app.h"
#include "main.h"
#include "tim.h"

static Ppm_receiver ppmReceiver;

/* STATIC FUNCTION PROTORYPES */

/* FUNCTION DEFINITION */

/**
 * @brief		This function calculate the difference in TIM2 tick between two rising edge of a PPM signal
 * 				Remember that PPM signals have the same 1ms to 2ms duration, but it is measured between two rising edge, and not between a rising and falling edges
 * @visibility	Everywhere
 * @param1		edge: edge detected, 0 falling correspond to GPIO_PIN_RESET, 1 rising correspond to GPIO_PIN_SET
 */
void FAC_ppm_receiver_Callback(uint8_t edge) {
	uint32_t t = __HAL_TIM_GET_COUNTER(&htim2);
	if (edge == RISING) {
		uint16_t dt = (uint16_t) (t - ppmReceiver.t1);
		if (dt > PPM_SYNC_LENGTH) {
			ppmReceiver.raw_channel[ppmReceiver.next_channel] = dt;
			ppmReceiver.next_channel = 0;
		} else {
			ppmReceiver.raw_channel[ppmReceiver.next_channel] = dt;
			ppmReceiver.next_channel += 1;
		}
		if (ppmReceiver.next_channel == PPM_RECEIVER_CHANNELS_NUMBER + 1) {
			ppmReceiver.next_channel = 0;
		}
		ppmReceiver.t1 = t;
	}
}

/**
 * @brief		Calculate the correct channel value from 0 to RECEIVER_CHANNEL_RESOLUTION. It will send it to the std_receiver object
 * @visibility	everyweher
 * @retval		the value of the channel
 * @note		TIM2 has a resolution of 0.5us (~35 min of period), not used the FAC_std_receiver_new_channel_value in rage return value
 */
void FAC_ppm_receiver_calculate_channels_values() {
	for (int i = 0; i < PPM_RECEIVER_CHANNELS_NUMBER; i++) {
		uint16_t value = ppmReceiver.raw_channel[i];
		if (value <= MAX_TIM2_TEORETICAL_CHANNEL_COUNT + (MAX_TIM2_TEORETICAL_CHANNEL_COUNT / 100) * 10) { // if the count is grater than a 10% the value is ignored it must be something wrong with the time capture
			if (value < MAX_TIM2_TEORETICAL_CHANNEL_COUNT / 2)
				value = MAX_TIM2_TEORETICAL_CHANNEL_COUNT / 2; // sometime the transmitter has a grater range than the expected, so the min value is set to 1000 to not have any problem on the calculus
			value = map_uint32(value - (MAX_TIM2_TEORETICAL_CHANNEL_COUNT / 2), 0, MAX_TIM2_TEORETICAL_CHANNEL_COUNT / 2, 0, RECEIVER_CHANNEL_RESOLUTION - 1);
			// if(value > RECEIVER_CHANNEL_RESOLUTION-1) value = RECEIVER_CHANNEL_RESOLUTION-1;	// not needed the value is already cropped on the map_uint32_t function
			FAC_std_receiver_new_channel_value(i + 1, value);
		}
	}

}

/**
 * @brief Initialize the ppmReceiver object and starts the tim2
 *
 */
void FAC_ppm_receiver_init() {
	HAL_TIM_Base_Start(&htim2);

	ppmReceiver.t1 = 0;
	ppmReceiver.next_channel = 0;
	for (int i = 0; i < PPM_RECEIVER_CHANNELS_NUMBER + 1; i++) {
		ppmReceiver.raw_channel[i] = 0;
	}
}
