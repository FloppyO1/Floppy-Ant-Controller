/*
 * fac_adc.h
 *
 *  Created on: Jul 28, 2025
 *      Author: filippo-castellan
 */

#ifndef INC_FAC_CODE_FAC_ADC_H_
#define INC_FAC_CODE_FAC_ADC_H_

#define ADC_VREF 3300000	// 3.3V = 3300000uV

typedef struct Adc {
	uint16_t resolution;	// ex: 12bit -> 4096
	uint32_t uVref;	// ex: 3.3V -> 3300000	(uV)
} Adc;

uint16_t FAC_adc_GET_resolution();
uint32_t FAC_adc_GET_Vref_in_uV();
uint16_t FAC_adc_get_raw_channel_value(uint8_t chNumber);
HAL_StatusTypeDef FAC_adc_Init();

#endif /* INC_FAC_CODE_FAC_ADC_H_ */
