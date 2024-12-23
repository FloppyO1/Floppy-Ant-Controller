/*
 * reciever.c
 *
 *  Created on: Jul 7, 2024
 *      Author: Filippo Castellan
 */
#include "reciever.h"

#define TIMCLOCK   48	// [MHz]
#define PRESCALAR  24

extern uint_fast8_t recieverIsPWM;
extern TIM_HandleTypeDef htim6;
extern uint16_t speed_steps;	// number of steps generated from the pwm and received from the receiver

uint16_t usWidth = 0;

/* Measure PPM */
uint8_t channelToRead = 1;
uint16_t reciever_channels[NUMBER_OF_PPM_CHANNEL + 1]; // channel values from ch1-ch6 (1-6) (7-8 with flysky-i6 not used) {0 is the separation pulse}

uint16_t getChannelValuePercentage(uint8_t channelNumber) {
	int temp = (reciever_channels[channelNumber] - 1000) / (1000 / speed_steps);
	if (temp > speed_steps) temp = speed_steps;	// clip to the maximum
	if (temp < 0) temp = 0;	// clip to the minimum
	return temp;
}

void initReciever(TIM_HandleTypeDef *htim) {
	HAL_TIM_Base_Start(htim);
}

uint8_t aligned = 0;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (recieverIsPWM) {
		// channel 1
		if (GPIO_Pin == CH1_Pin) {
			if (HAL_GPIO_ReadPin(CH1_GPIO_Port, CH1_Pin) == 1) {	// rising edge
				aligned = 0;
				__HAL_TIM_SET_COUNTER(&htim6, 0);
				aligned = HAL_GPIO_ReadPin(CH2_GPIO_Port, CH2_Pin);	// if the second channel are high means that all the channels are aligned
			} else {	// falling edge
				usWidth = __HAL_TIM_GET_COUNTER(&htim6) / (TIMCLOCK / PRESCALAR);
				reciever_channels[1] = usWidth;
			}
		}
		if (aligned) {
			// channel 2 (only falling, because it starts with ch1)
			if (GPIO_Pin == CH2_Pin) {
				usWidth = __HAL_TIM_GET_COUNTER(&htim6);
				reciever_channels[2] = usWidth;
			}
			// channel 3 (only falling, because it starts with ch1)
			if (GPIO_Pin == CH3_Pin) {
				usWidth = __HAL_TIM_GET_COUNTER(&htim6);
				reciever_channels[3] = usWidth;
			}
			// channel 4 (only falling, because it starts with ch1)
			if (GPIO_Pin == CH4_Pin) {
				usWidth = __HAL_TIM_GET_COUNTER(&htim6);
				reciever_channels[4] = usWidth;
			}
		} else {	// not aligned, subtract channel 1 time from ch2 time etc.
			// channel 2 (only falling, because it starts after ch1)
			if (GPIO_Pin == CH2_Pin) {
				usWidth = __HAL_TIM_GET_COUNTER(&htim6);
				reciever_channels[2] = usWidth - (reciever_channels[1]);
			}
			// channel 3 (only falling, because it starts after ch2)
			if (GPIO_Pin == CH3_Pin) {
				usWidth = __HAL_TIM_GET_COUNTER(&htim6);
				reciever_channels[3] = usWidth - (reciever_channels[1] + reciever_channels[2]);
			}
			// channel 4 (only falling, because it starts after ch3)
			if (GPIO_Pin == CH4_Pin) {
				usWidth = __HAL_TIM_GET_COUNTER(&htim6);
				reciever_channels[4] = usWidth - (reciever_channels[1] + reciever_channels[2] + reciever_channels[3]);
			}
		}

	} else {
		if (GPIO_Pin == CH1_Pin) {
			if (HAL_GPIO_ReadPin(CH1_GPIO_Port, CH1_Pin) == 0) {
				usWidth = __HAL_TIM_GET_COUNTER(&htim6) / (TIMCLOCK/PRESCALAR);
				__HAL_TIM_SET_COUNTER(&htim6, 0);  // reset the counter
				if (usWidth > 4500) { // if the separation pulse found, the next pulse is ch1
					reciever_channels[0] = usWidth;	// store the separation pulse width
					channelToRead = 1;	// next channel is ch1
				} else {	// if the pulse is a channel (so < 6ms)
					if (channelToRead != 0) {
						reciever_channels[channelToRead] = usWidth;	// store the current channel width
						channelToRead++;
						if (channelToRead > NUMBER_OF_PPM_CHANNEL) {
							channelToRead = 0;
						}
					}
				}
			}
		}
	}
}

TankMotors tankMix(uint16_t STchannel, uint16_t THchannel) {
	TankMotors t;
	int16_t st = (STchannel - speed_steps/2) * 2;
	int16_t th = (THchannel - speed_steps/2) * 2;
	int16_t ml = th + st;	// mixing
	int16_t mr = th - st;	// mixing
	// keep the value inside -speed_steps and speed_steps
	if (ml > speed_steps) ml = speed_steps;
	if (ml < -speed_steps) ml = -speed_steps;
	if (mr > speed_steps) mr = speed_steps;
	if (mr < -speed_steps) mr = -speed_steps;
	t.speedL = (ml + speed_steps) / 2;	// keep value inside 0 and speed_steps
	t.speedR = (mr + speed_steps) / 2;
	return t;
}
