/*
 * reciever.h
 *
 *  Created on: Jul 7, 2024
 *      Author: Filippo Castellan
 */

#ifndef INC_RECIEVER_H_
#define INC_RECIEVER_H_

#include "main.h"
#include "motor.h"

/* --------- FUNCTION DECLARATION ---------- */
uint8_t getChannelValuePercentage(uint8_t channelNumber); // compute the channel value
//void initRecieverPPM(TIM_HandleTypeDef *htim); // not needed any more
void initReciever(TIM_HandleTypeDef *htim);
struct TankMotors tankMix(uint8_t STchannel, uint8_t THchannel);

#endif /* INC_RECIEVER_H_ */
