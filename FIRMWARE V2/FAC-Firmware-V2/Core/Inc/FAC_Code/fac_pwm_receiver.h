/*
 * fac_pwm_receiver.h
 *
 *  Created on: Aug 3, 2025
 *      Author: filippo-castellan
 */

#ifndef INC_FAC_CODE_FAC_PWM_RECEIVER_H_
#define INC_FAC_CODE_FAC_PWM_RECEIVER_H_

#include "stm32f0xx_hal.h"
#include "FAC_Code/config.h"

#define PWM_RECEIVER_CHANNELS_NUMBER 4


typedef struct Pwm_receiver {
		uint32_t channels_t1[PWM_RECEIVER_CHANNELS_NUMBER];	// 32 bit because the tim2 used has 32bit of resolution
		uint32_t channels_t2[PWM_RECEIVER_CHANNELS_NUMBER];
} Pwm_receiver;


void FAC_pwm_receiver_Callback(uint8_t edge, uint16_t GPIO_Pin);
void FAC_pwm_receiver_calculate_channel_value(uint8_t chNumber);
void FAC_pwm_receiver_init();

#endif /* INC_FAC_CODE_FAC_PWM_RECEIVER_H_ */
