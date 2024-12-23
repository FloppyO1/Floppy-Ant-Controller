/*
 * DMApwm.h
 *
 *  Created on: Dec 23, 2024
 *      Author: Filippo Castellan
 *
 * 	Some of the code is taken from https://github.com/javiBajoCero/softPWMtutorial.git
 * 	https://www.hackster.io/javier-munoz-saez/all-pins-as-pwm-at-the-same-time-baremetal-stm32-1df86f#toc-dma-3
 */


#ifndef INC_DMAPWM_H_
#define INC_DMAPWM_H_

#include "stm32f0xx_hal.h"

/*
 * 			SETTINGS
 * 	Change the following settings according to your deeds
 * 	PWM_STEPS: number of steps that a period will be subdivided
 * 	PWM_FREQ: frequency of the PWM signal
 * 	TIMER_FREQ: frequency at which the timer runs
 */
#define PWM_STEPS 100
#define PWM_FREQ 1000	// freq is given by tim_freq/ARR/lengthSoftPWMbuffer
#define TIMER_FREQ 48000000	// freq at which timers are running [Hz]


// only for code easyer to read
#define MAX_DMA_PWM_VALUE PWM_STEPS
#define MIN_DMA_PWM_VALUE 0

void initDMApwm();
uint8_t setDMApwmDuty(GPIO_TypeDef* port, uint16_t pin, uint16_t duty);
void setSoftPWM(uint16_t pin, uint32_t duty, uint32_t *softpwmbuffer);
void zeroSoftPWM(uint32_t softpwmbuffer[]);

#endif /* INC_DMAPWM_H_ */

