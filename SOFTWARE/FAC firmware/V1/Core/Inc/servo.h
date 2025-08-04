/*
 * servo.h
 *
 *  Created on: Jul 7, 2024
 *      Author: Filippo Castellan
 */

#ifndef INC_SERVO_H_
#define INC_SERVO_H_

#include "main.h"

void setServoIsEnable(uint8_t state, TIM_HandleTypeDef *htim);
void setServoAngle(uint8_t servoN, uint16_t percentage);

enum SERVO{ S1, S2};

#endif /* INC_SERVO_H_ */
