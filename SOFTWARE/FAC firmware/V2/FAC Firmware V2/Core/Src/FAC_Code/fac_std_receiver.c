/*
 * fac_std_receiver.c
 *
 * This file contains all the functions that manage the standard
 * receiver model. The FAC_app references this file to obtain the
 * values of the various receiver channels. The implementation of
 * signal reading for the various receiver types has been done in
 * separate files.
 *
 * N.B. Each type of receiver must provide the receiver object
 * 		in this file with the values of the various channels.
 *
 *  Created on: Aug 3, 2025
 *      Author: filippo-castellan
 */

#include "main.h"

#include "FAC_Code/fac_std_receiver.h"
#include "FAC_Code/fac_app.h"
#include "FAC_Code/fac_settings.h"
#include "FAC_Code/config.h"
#include "FAC_Code/fac_pwm_receiver.h"
#include "FAC_Code/fac_ppm_receiver.h"

Std_receiver receiver;

/* STATIC FUNCTION PROTORYPES */
static uint16_t FAC_std_receiver_SET_channel(uint8_t chNumber, uint16_t value);

/* FUNCTION DEFINITION */
/**
 * @brief 		Set a value on the receiver "object" a channel value, keeping it inside the correct range
 * @visibility	Only on this file
 * @note 		This function has directly access to the receiver object
 */
static uint16_t FAC_std_receiver_SET_channel(uint8_t chNumber, uint16_t value) {
	uint16_t v = value;
	if (v > RECEIVER_CHANNEL_RESOLUTION - 1) {	// -1 because number 0 is considered so 1000 step means 0 to 999
		v = RECEIVER_CHANNEL_RESOLUTION - 1;
	}
	receiver.channels[chNumber - 1] = v;
	return v;
}

/*
 * @brief	This function calculate the new value applying a deadzone in the center (0) and at the extremes
 * @note 	Center deadzone applied only in case the value is positive and negative. Channel 3 only has exremes deadzone
 * @arg1	Value is the value given by the std reciver [0, RECEIVER_RESOLUTION]
 *
 *               __
 *              /	 <-- high deadzone
 *             /
 *            /
 *           /
 *         --		<-- center deadzone
 *        /
 *       /
 *      /
 *   __/			<-- low deadzone
 *
 */
static int16_t FAC_std_receiver_calculate_dead_zone(uint16_t value, uint8_t deadzonePerc, uint8_t chNumber) {
	int16_t temp = (int16_t) value;
	int16_t maxValue = RECEIVER_CHANNEL_RESOLUTION;
	int16_t minValue = -RECEIVER_CHANNEL_RESOLUTION;
	uint16_t deadzoneValue = ((maxValue - minValue) / 100 / 2) * deadzonePerc; // calculate the deadzone using the precentage argument on the full range
	/* keep the value inside the range */
	if (chNumber != 3) {	// channel 3 always don't have the return spring, so the offset must be only at the extremes
		temp = map_int32(temp, 0, RECEIVER_CHANNEL_RESOLUTION, -RECEIVER_CHANNEL_RESOLUTION, RECEIVER_CHANNEL_RESOLUTION); // change the range of the value
	}

	// keep inside the max e min value
	if (value > maxValue)
		temp = maxValue;
	if (value < minValue)
		temp = minValue;

	// extremes deadzone
	if (temp + (deadzoneValue * 2) >= maxValue)
		temp = maxValue;
	else if (temp - (deadzoneValue * 2) <= minValue)
		temp = minValue;

	else if (temp >= -deadzoneValue && temp <= deadzoneValue)
		temp = 0;	// the value is inside the center deadzone

	/* linearization of the value if it is not in the center */
	else if (temp > 0)	// linearize the top value
		temp = map_int32(temp, 0 + deadzoneValue, maxValue - (deadzoneValue * 2), 0, maxValue);
	else if (temp < 0)	// linearize the bottom value
		temp = map_int32(temp, minValue + (deadzoneValue * 2), 0 - deadzoneValue, minValue, 0);

	if (chNumber != 3) {
		temp = map_int32(temp, -RECEIVER_CHANNEL_RESOLUTION, RECEIVER_CHANNEL_RESOLUTION, 0, RECEIVER_CHANNEL_RESOLUTION);
	}

	return temp;
}

/*
 * @brief calculate the new value of the requested channel
 * @retval return the new channel value if it was correct, otherwise return the old value
 */
uint16_t FAC_std_receiver_GET_channel(uint8_t chNumber) {
	switch (receiver.type) {	// calculate channel / channels according to the type of receiver in use
		case RECEIVER_TYPE_PWM:
			if (chNumber <= PWM_RECEIVER_CHANNELS_NUMBER)// update the channel only if the channel exist on this rx type (else it returns the channel number stored on std_receiver without update)
				FAC_pwm_receiver_calculate_channel_value(chNumber);		// calculate the value of the channel requested
			break;
		case RECEIVER_TYPE_PPM:
			if (chNumber <= PPM_RECEIVER_CHANNELS_NUMBER)// update the channel only if the channel exist on this rx type (else it returns the channel number stored on std_receiver without update)
				FAC_ppm_receiver_calculate_channels_values();
			break;
		case RECEIVER_TYPE_NRF24:

			break;
		case RECEIVER_TYPE_ELRS:

			break;
	}
	return receiver.channels[chNumber - 1];	// - 1 because the array start from 0 and ands at RECEIVER_CHANNLES_NUMBER-1
}

/**
 * @brief 		Allows the different types of receiver to set the new value of a specific channel
 * @visibility	Everywhere
 * @retval		Return 0 if the new value was inside the range, 1 if it was outside the range so it is resized to max value allowed
 */
uint8_t FAC_std_receiver_new_channel_value(uint8_t chNumber, uint16_t value) {
	uint16_t valueWithDeadzone = FAC_std_receiver_calculate_dead_zone(value, FAC_settings_GET_value(FAC_SETTINGS_CODE_CHANNELS_DEADZONE_PERCENTAGE), chNumber);
	uint16_t valueStored = FAC_std_receiver_SET_channel(chNumber, valueWithDeadzone);
	if (value != valueStored)
		return 1;	// the value stored was not in the range (value stored do not correspond to "value")
	return 0;
}

/**
 * @brief 	Initialize the std_reciever with all channels to zero, and initialize the correct receiver type
 * @visibility	Everywhere
 * @note  	It must be used at the firmware start up only
 */
void FAC_std_reciever_init(uint8_t type) {
	for (int i = 0; i < RECEIVER_CHANNELS_NUMBER; i++) {	// initialize all the channels value to zero
		receiver.channels[i] = 0;
	}
	receiver.type = type;
	switch (receiver.type) {
		case RECEIVER_TYPE_PWM:
			FAC_pwm_receiver_init();
			break;
		case RECEIVER_TYPE_PPM:
			FAC_ppm_receiver_init();
			break;
		case RECEIVER_TYPE_NRF24:
			// INITIALZE THIS TYPE OF RECEIVER..
			break;
		case RECEIVER_TYPE_ELRS:
			// INITIALZE THIS TYPE OF RECEIVER..
			break;
	}

	/* stay in this loop until some channel are received */
	uint8_t receiverConnected = FALSE;
	uint8_t channelToCheck = 0;
	do {
		if(channelToCheck == 0) channelToCheck++;
		else channelToCheck = ((channelToCheck+1) % (RECEIVER_CHANNELS_NUMBER+1));
		if (FAC_std_receiver_GET_channel(channelToCheck) != 0)
			receiverConnected = TRUE;
	} while (!receiverConnected);
}

/**
 * @brief 		This function redirect the callback of the HW channle pins to the correct type of receiver function
 * @visibility	Everywhere
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	/* check the edge that triggered the interrupt */
	GPIO_PinState edge;
	switch (GPIO_Pin) {
		case CH4_Pin:	// the only pin with port A
			edge = HAL_GPIO_ReadPin(CH4_GPIO_Port, CH4_Pin);
			break;
		default:	// other pins has port B
			edge = HAL_GPIO_ReadPin(GPIOB, GPIO_Pin);
			break;
	}

	switch (receiver.type) {
		case RECEIVER_TYPE_PWM:
			FAC_pwm_receiver_Callback(edge, GPIO_Pin);
			break;
		case RECEIVER_TYPE_PPM:
			if (GPIO_Pin == CH1_Pin)	// only on ch1 arrives ppm signal from FS2A receiver
				FAC_ppm_receiver_Callback(edge);
			break;
			// NOT USED FOR NRF24 AND ELRS BECAUSE THEY USE A SERIA CONNECTION TO COMUNICATE
	}
}
