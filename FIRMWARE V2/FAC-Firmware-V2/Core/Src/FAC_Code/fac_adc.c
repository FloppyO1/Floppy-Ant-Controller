/*
 * fac_adc.c
 *
 *  Created on: Jul 28, 2025
 *      Author: filippo-castellan
 */

#include "stm32f0xx_hal.h"
#include "adc.h"

#include "FAC_Code/fac_adc.h"

/* defines */
#define FAC_ADC_CH_VBAT 0
#define FAC_ADC_CH_AAUX 1

/* variable */
static Adc adc;
static uint16_t ADC_values[2];	// only visible on this file

/* STATIC FUNCTION PROTORYPES */


/* FUNCTION DEFINITION */
/**
 * @bried 	Get the resolution of the adc
 * @retval 	Returns the adc resolution
 */
uint16_t FAC_adc_GET_resolution() {
	return adc.resolution;
}

/**
 * @bried 	Get the vref in uV of the adc
 * @retval 	Returns the vref in uV of the adc
 */
uint32_t FAC_adc_GET_Vref_in_uV() {
	return adc.uVref;
}

/**
 * @brief 	Initialize the ADC module
 * @retval 	Status of the initialization in HAL_StatusTypeDef form
 */
HAL_StatusTypeDef FAC_adc_Init() {
	HAL_StatusTypeDef EndState = HAL_OK;
	/* write the code here - START */
	HAL_Delay(100);	// wait some time to allow the power supply to stabilize its output
	HAL_ADCEx_Calibration_Start(&hadc);
	HAL_Delay(50);
	EndState = HAL_ADC_Start_DMA(&hadc, ADC_values, 2);

	adc.uVref = ADC_VREF;	// vref of 3.3V = 3300000uV
	adc.resolution = 2;
	switch (hadc.Init.Resolution) {
		case ADC_RESOLUTION_12B:
			adc.resolution <<= 12-1; // raise 2 at the power of hadc resolution
			break;
		case ADC_RESOLUTION_10B:
			adc.resolution <<= 10-1;
			break;
		case ADC_RESOLUTION_8B:
			adc.resolution <<= 8-1;
			break;
		case ADC_RESOLUTION_6B:
			adc.resolution <<= 6-1;
			break;
	}

	/* write the code here - END */
	return EndState;
}

/**
 * @brief 		Give the raw adc value of a single channel
 * @param 		chNumber: Channel number of the channel you want to read
 * @visibility 	All files
 * @retval 		Raw value of the chNumber selected
 */
uint16_t FAC_adc_get_raw_channel_value(uint8_t chNumber) {	// only visible on this file
	return ADC_values[chNumber];
}

/**
 * @brief 	Only if we need to process data at the end of the ADC conversion (we don't)
 * @note 	This is a pre-defined interrupt callback function
 */
//void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
//	// Process the data if needed
//}
