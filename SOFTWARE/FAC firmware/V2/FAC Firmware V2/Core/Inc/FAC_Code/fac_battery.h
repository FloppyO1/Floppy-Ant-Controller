/*
 * fac_battery.h
 *
 *  Created on: Jul 31, 2025
 *      Author: filippo-castellan
 */

#ifndef INC_FAC_CODE_FAC_BATTERY_H_
#define INC_FAC_CODE_FAC_BATTERY_H_

#include "stm32f0xx_hal.h"

#define ADC_BATTERY_CHANNEL 0
/* BATTERY CORRECTION FACTOR */
#define BATTERY_VOLTAGE_CORRECTION_M_FACTOR 1.025f
#define BATTERY_VOLTAGE_CORRECTION_Q_FACTOR -0.125f

/* BATTERY CELL DETECTION RANGES*/
#define NOMINAL_BATTERY_LEVEL  3800	//3.8V to have 0.4V form 4.2V max and 3.4V theoretical min
#define TYPIC_BATTERY_RANGE 450	// the 0.4V mentioned above + 50 mV of tollerance
#define TYPIC_DIODE_VOLTAGE_DROP 300	// used to detect usb powered case
#define NOMINAL_USB_VOLTAGE 5100
#define USB_POWER_TOLLERACE	200	// used to detect usb powered case

typedef struct Battery{
		uint16_t voltage;	// V*100 (8.02V -> 802)
		uint16_t single_cell_voltage;
		uint8_t type;	// number of cells detected
		uint8_t low_battery_state;	// TRUE if the battery voltage is below a certain "limit" voltage
		uint8_t cut_off_state;	// TRUE if the battery voltage is below a certain "cutoff" voltage
		uint16_t voltage_divider_ratio;	// voltage divider ratio on the hardware (ex: 7.69 -> 7690
} Battery;

enum BATTERY_TYPE {
	BATTERY_TYPE_USB,
	BATTERY_TYPE_1S,
	BATTERY_TYPE_2S,
	BATTERY_TYPE_3S,
	BATTERY_TYPE_4S,
	BATTERY_TYPE_NONE
};

void FAC_battery_init();
uint16_t FAC_battery_GET_voltage ();
uint16_t FAC_battery_GET_voltage();
uint16_t FAC_battery_GET_cell_voltage();
uint16_t FAC_battery_GET_type();


#endif /* INC_FAC_CODE_FAC_BATTERY_H_ */
