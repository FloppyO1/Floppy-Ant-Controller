/*
 * fac_std_receiver.h
 *
 *  Created on: Aug 3, 2025
 *      Author: filippo-castellan
 */

#ifndef INC_FAC_CODE_FAC_STD_RECEIVER_H_
#define INC_FAC_CODE_FAC_STD_RECEIVER_H_

#include "stm32f0xx_hal.h"
#include "FAC_Code/config.h"

#define MAX_TIM2_TEORETICAL_CHANNEL_COUNT 4000	// one tick every 0.5us, so 2ms = 2000 ticks

typedef struct Std_receiver {
		uint16_t channels[RECEIVER_CHANNELS_NUMBER];		// real 0-max channels values
		uint8_t type;	// follow the type enum, indicate witch type of receiver was initialized
} Std_receiver;

enum RECEIVER_TYPE{
	RECEIVER_TYPE_PWM,
	RECEIVER_TYPE_PPM,
	RECEIVER_TYPE_NRF24,
	RECEIVER_TYPE_ELRS
};

enum EDGE{
	FALLING,	// 0 falling correspond to GPIO_PIN_RESET
	RISING		// 1 rising correspond to GPIO_PIN_SET
};

uint16_t FAC_std_receiver_GET_channel (uint8_t chNumber);
uint8_t FAC_std_receiver_new_channel_value(uint8_t chNumber, uint16_t value);
void FAC_std_reciever_init(uint8_t type);


#endif /* INC_FAC_CODE_FAC_STD_RECEIVER_H_ */
