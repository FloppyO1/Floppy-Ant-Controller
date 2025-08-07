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

#define EEPROM_ADDRESS 0xA0				// device address

#define EEPROM_DATA_BEGIN_ADDRESS 0		// data address
#define EEPROM_ISFIRSTBOOT_ADDRESS 100	// is first boot data address

#endif /* INC_FAC_CODE_FAC_EEPROM_H_ */
