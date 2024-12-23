/*
 * servo.c
 *
 *  Created on: Jul 7, 2024
 *      Author: Filippo Castellan
 */

#include "servo.h"

extern uint8_t s1Rev;
extern uint8_t s2Rev;

uint8_t getServoRev(uint8_t servoN) {	// return 1 if the motorN is reversed, else 0
	uint8_t isRev = 0;
	switch (servoN) {
		case S1:
			isRev = s1Rev;
			break;
		case S2:
			isRev = s2Rev;
			break;
	}
	return isRev;
}


/*
 * to calculate the factor, use this formula:
 * P = 1/FREQ
 * P(us) = P * 1'000'000
 * factor = 10000/P(us)
 * if factor < 1 divide by the new factor
 * new factor = 1/factor
 */
void setServoAngle(uint8_t servoN, uint8_t percentage) {
	const uint8_t factor = 2;
	uint16_t temp = calculateSpeedWithDeadZoneDouble(percentage);
	if (temp > 100) temp = 100;
	if (temp < 0) temp = 0;

	if (getServoRev(servoN)) temp = 100 - temp;	// take in count if the servo must be reversed or not

	switch (servoN) {
		case S1:
			temp = map(temp, 0, 100, S1_MIN_TIME_PWM, S1_MAX_TIME_PWM);
			TIM16->CCR1 = temp * factor;
			break;
		case S2:
			temp = map(temp, 0, 100, S2_MIN_TIME_PWM, S2_MAX_TIME_PWM);
			TIM17->CCR1 = temp * factor;
			break;
	}
}

void setServoIsEnable(uint8_t state, TIM_HandleTypeDef *htim){
	if(state == TRUE){	// turn of the pwm generation
		HAL_TIMEx_PWMN_Start(htim,TIM_CHANNEL_1);	// can be TIM16 or TIM17, channel 1 used on all of them
	}else{
		HAL_TIMEx_PWMN_Stop(htim,TIM_CHANNEL_1);	// can be TIM16 or TIM17, channel 1 used on all of them
	}
}
