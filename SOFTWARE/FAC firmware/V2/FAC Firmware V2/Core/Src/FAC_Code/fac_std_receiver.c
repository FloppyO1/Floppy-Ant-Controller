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
	uint16_t valueStored = FAC_std_receiver_SET_channel(chNumber, value);
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
