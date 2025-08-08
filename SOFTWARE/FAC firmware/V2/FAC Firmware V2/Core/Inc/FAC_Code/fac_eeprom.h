/*
 * fac_eeprom.h
 *
 *  Created on: Aug 7, 2025
 *      Author: filippo-castellan
 */

#ifndef INC_FAC_CODE_FAC_EEPROM_H_
#define INC_FAC_CODE_FAC_EEPROM_H_

#include "stm32f0xx_hal.h"
#include "FAC_Code/config.h"

#define EEPROM_ADDRESS 0xA0			// device address
#define EEPROM_SIZE_IN_BIT 2000		// 2kBit
#define EEPROM_SETTINGS_SIZE 2		// 2 byte for each setting

#define EEPROM_DATA_BEGIN_ADDRESS 0		// data address
#define EEPROM_ISFIRSTBOOT_ADDRESS 255	// is first boot data address, MUST BE A BIG VALUE OTHERWIS SETTINGS VALUES ARE OVERVRITTEN
#define EEPROM_MAX_SETTINGS_NUMBER (EEPROM_SIZE_IN_BIT/8/2)-1

typedef struct Eeprom {
	uint8_t is_first_boot_value;
} Eeprom;

void FAC_eeprom_store_value(uint8_t position, uint16_t value);
uint16_t FAC_eeprom_read_value(uint8_t position);
uint8_t FAC_eeprom_is_first_time();
void FAC_eeprom_init(uint8_t bootValue);
uint8_t FAC_eeprom_GET_is_first_boot_value();
void FAC_eeprom_WRITE_frist_boot_value_in_eeprom();

#endif /* INC_FAC_CODE_FAC_EEPROM_H_ */
