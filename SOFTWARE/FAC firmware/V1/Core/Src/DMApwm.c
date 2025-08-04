/*
 * DMApwm.c
 *
 *  Created on: Dec 23, 2024
 *      Author: Filippo Castellan
 *
 *  Some of the code is taken from https://github.com/javiBajoCero/softPWMtutorial.git
 *  https://www.hackster.io/javier-munoz-saez/all-pins-as-pwm-at-the-same-time-baremetal-stm32-1df86f#toc-dma-3
 */
#include "DMApwm.h"
// ---------------------------------------------------------------------------------------------------------------
// 				!!!!!!!!!!!!!	 ATTENCTION THIS CODE WORKS ONLY FOR PORTs A AND B	  !!!!!!!!!!!!!!!!!
// ---------------------------------------------------------------------------------------------------------------

/*	HOW TO SET UP TIMERs AND DMAs
 * 1) find 2 timers that can access via DMA to the BSRR register (TIM1, TIM2, TIM4 should be good)
 * 2) set them with a clock sourca (internal one is ok)
 * 3) on the parameter settings of the each timer set to Update Event the TRGO
 * 4) activate for each timer the DMA
 * 5) set DMA channel to transfer data from memory to peripheral, a Circular array of Words
 * 6) in this file change the timers and DMAs hendle type as described below
 * 7) on the DMApwm.h file set the desired PWM_STEPS and PWM_FREQ as long as TIMER_FREQ
 * 8) you are good to go, all the GPIO where you want the PWM must be part of A or B port and must be setted as GPIO_Output
 */

// SOME SETTINGS ARE INSIDE THE DMApwm.h file

// timer handle type and hdma handle type
// INSERT YOUR HADLE TYPE NAME making find&replace in all the document (Ctrl+F)
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;
extern DMA_HandleTypeDef hdma_tim1_ch3_up;
extern DMA_HandleTypeDef hdma_tim3_ch4_up;

// buffer for the GPIO values (data transfered by DMA)
uint32_t dataA[PWM_STEPS];
uint32_t dataB[PWM_STEPS];

void initDMApwm() {
	// set the frequency
	htim1.Init.Period = TIMER_FREQ - 1;
	htim1.Instance->ARR = (TIMER_FREQ / (PWM_STEPS * PWM_FREQ)) - 1;
	htim3.Init.Period = TIMER_FREQ - 1;
	htim3.Instance->ARR = (TIMER_FREQ / (PWM_STEPS * PWM_FREQ)) - 1;
	// start timers
	HAL_TIM_Base_Start(&htim1);
	HAL_TIM_Base_Start(&htim3);

	// configure DMAs
	HAL_DMA_Start(&hdma_tim1_ch3_up, (uint32_t) &(dataA[0]), (uint32_t) &(GPIOA->BSRR), sizeof(dataA) / sizeof(dataA[0]));
	HAL_DMA_Start(&hdma_tim3_ch4_up, (uint32_t) &(dataB[0]), (uint32_t) &(GPIOB->BSRR), sizeof(dataB) / sizeof(dataB[0]));

	// starts DMAs
	__HAL_TIM_ENABLE_DMA(&htim1, TIM_DMA_UPDATE);
	__HAL_TIM_ENABLE_DMA(&htim3, TIM_DMA_UPDATE);

	// set the PWMs to 0
	zeroSoftPWM(dataA);
	zeroSoftPWM(dataB);
}

uint8_t setDMApwmDuty(GPIO_TypeDef *port, uint16_t pin, uint16_t duty) {
	uint8_t r = 0;
	if (port == GPIOA) {
		setSoftPWM(pin, duty, (uint32_t*) &dataA);
		r = 1;
	}
	if (port == GPIOB) {
		setSoftPWM(pin, duty, (uint32_t*) &dataB);
		r = 1;
	}
	return r;	// 1 = ok, 0 = no compatible port found
}

void setSoftPWM(uint16_t pin, uint32_t duty, uint32_t *softpwmbuffer) {
	for (uint32_t i = 0; i < PWM_STEPS; ++i) {
		if (i < duty) { //set pin
			softpwmbuffer[i] &= (uint32_t) ~(pin << 16);
			softpwmbuffer[i] |= (uint32_t) pin;
		} else { //reset pin
			softpwmbuffer[i] &= (uint32_t) ~(pin);
			softpwmbuffer[i] |= (uint32_t) pin << 16;
		}
	}
}

void zeroSoftPWM(uint32_t softpwmbuffer[]) {
	for (uint32_t i = 0; i < PWM_STEPS; ++i) {
		softpwmbuffer[i] = 0;
	}
}
