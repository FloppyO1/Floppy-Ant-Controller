/*
 * reciever.c
 *
 *  Created on: Jul 7, 2024
 *      Author: Filippo Castellan
 */
#include "reciever.h"

#define TIMCLOCK   48000000
#define PRESCALAR  48

extern uint_fast8_t recieverIsPWM;
extern TIM_HandleTypeDef htim6;

uint16_t usWidth = 0;

/* Measure PPM */
uint8_t channelToRead = 1;
uint16_t reciever_channels[NUMBER_OF_PPM_CHANNEL + 1]; // channel values from ch1-ch6 (1-6) (7-8 with flysky-i6 not used) {0 is the separation pulse}


// the capture compare mode are not used, replaced with external interrupt to allow normal PWM channel mode
//void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) { // the correct timer is given from who trigger the interrupt
//	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3) { // if the interrupt source is channel1
//		usWidth = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);
//		__HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter
//		if (usWidth > 4500) { // if the separation pulse found, the next pulse is ch1
//			reciever_channels[0] = usWidth;	// store the separation pulse width
//			channelToRead = 1;	// next channel is ch1
//		} else {	// if the pulse is a channel (so < 6ms)
//			if (channelToRead != 0) {
//				reciever_channels[channelToRead] = usWidth;	// store the current channel width
//				channelToRead++;
//				if (channelToRead > NUMBER_OF_PPM_CHANNEL) {
//					channelToRead = 0;
//				}
//			}
//		}
//	}
//}

uint8_t getChannelValuePercentage(uint8_t channelNumber) {
	int temp = (reciever_channels[channelNumber] - 1000) / 10;
	if (temp > 100) temp = 100;
	if (temp < 0) temp = 0;
	return temp;
}

//void initRecieverPPM(TIM_HandleTypeDef *htim) {
//	HAL_TIM_IC_Start_IT(htim, TIM_CHANNEL_3);
//}

void initReciever(TIM_HandleTypeDef *htim) {
	HAL_TIM_Base_Start(htim);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (recieverIsPWM) {
		// channel 1
		if (GPIO_Pin == CH1_Pin) {
			if (HAL_GPIO_ReadPin(CH1_GPIO_Port, CH1_Pin) == 1) {
				__HAL_TIM_SET_COUNTER(&htim6, 0);
			} else {
				usWidth = __HAL_TIM_GET_COUNTER(&htim6);
				reciever_channels[1] = usWidth;
			}
		}
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
	} else {
		if (GPIO_Pin == CH1_Pin) {
			if (HAL_GPIO_ReadPin(CH1_GPIO_Port, CH1_Pin) == 0) {
				usWidth = __HAL_TIM_GET_COUNTER(&htim6);
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

TankMotors tankMix(uint8_t STchannel, uint8_t THchannel) {
	TankMotors t;
	int8_t st = (STchannel - 50) * 2;
	int8_t th = (THchannel - 50) * 2;
	int16_t ml = th + st;	// mixing
	int16_t mr = th - st;	// mixing
	// keep the value inside -100 and 100
	if (ml > 100) ml = 100;
	if (ml < -100) ml = -100;
	if (mr > 100) mr = 100;
	if (mr < -100) mr = -100;
	t.speedL = (ml + 100) / 2;	// keep value inside 0 and 100
	t.speedR = (mr + 100) / 2;
	return t;
}
