/*
 * fac_ppm_receiver.h
 *
 *  Created on: Aug 3, 2025
 *      Author: filippo-castellan
 */

#ifndef INC_FAC_CODE_FAC_PPM_RECEIVER_H_
#define INC_FAC_CODE_FAC_PPM_RECEIVER_H_

#include "stm32f0xx_hal.h"
#include "FAC_Code/config.h"

#define PPM_RECEIVER_CHANNELS_NUMBER 8
#define PPM_SYNC_LENGTH  8000	// the length of the long low signal that separate each data packet (8ms = 8000us)


typedef struct Ppm_receiver {
		uint32_t t1;
		uint16_t raw_channel[PPM_RECEIVER_CHANNELS_NUMBER+1];
		uint8_t next_channel;
} Ppm_receiver;


void FAC_ppm_receiver_Callback(uint8_t edge);
void FAC_ppm_receiver_calculate_channels_values();
void FAC_ppm_receiver_init();


#endif /* INC_FAC_CODE_FAC_PPM_RECEIVER_H_ */
