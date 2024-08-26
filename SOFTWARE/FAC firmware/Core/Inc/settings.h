/*
 * settings.h
 *
 *  Created on: Jul 14, 2024
 *      Author: Filippo Castellan
 */

#ifndef INC_SETTINGS_H_
#define INC_SETTINGS_H_

#include "stm32f0xx_hal.h"

#define EEPROM_DATA_BEGIN_ADDRESS 0	// data address
#define EEPROM_ISFIRSTBOOT_ADDRESS 100	// isfirstboot data address

/* ------------- COMMANDs LIST ------------- */
#define COMMAND_TYPE_SEPARATOR ">"
//CHANNELs MAPPING
// TYPE (VALUE = number of the channel)
#define COMMAND_TH2CH "TH2CH"	// throttel to channel..
#define COMMAND_ST2CH "ST2CH"	// steering
#define COMMAND_WP2CH "WP2CH"	// weapon
#define COMMAND_S12CH "S12CH"	// servo 1 (HV)
#define COMMAND_S22CH "S22CH"	// servo 2
#define COMMAND_ARM "ARM2CH"		// arm robot
// DC MOTORs/ SERVOs REVERSE
// TYPE (VALUE = 0 if no reverse, 1 if reverse)
#define COMMAND_M1REV "M1REV"	// reverse the rotation of the motor
#define COMMAND_M2REV "M2REV"
#define COMMAND_M3REV "M3REV"
#define COMMAND_S1REV "S1REV"	// reverse the position of the servo
#define COMMAND_S2REV "S2REV"
// Mx ASSIGNMENT
// TYPE
#define COMMAND_M1ASS "M1ASS"	// assign the motor 1 to MOTOR LEFT, MOTOR, RIGHT, WEAPPON MOTOR
#define COMMAND_M2ASS "M2ASS"
#define COMMAND_M3ASS "M3ASS"
// VALUE
#define VALUE_ML "ML"
#define VALUE_MR "MR"
#define VALUE_WP "WP"
// WEAPPON DOUBLE/SINGLE DIRECTION
// TYPE (VALUE = 1 if double direction, 0 if single direction)
#define COMMAND_WPDD "WPDD"
// TANK MODE
// TYPE (VALUE = 0 if disabled, 1 if enabled)
#define COMMAND_TANKON "TNKON"
// OTHER
// TYPE
#define COMMAND_NODISARM "NODISARM" // VALUE = 0 if disabled, 1 if enabled
#define COMMAND_LIMITVOLTAGE "VLIM"	// VALUE = voltage*10 (3.3V -> 33)
#define COMMAND_CUTOFFVOLTAGE "VCUTOFF" // VALUE = voltage*10 (3.3V -> 33)
#define COMMAND_RXPWMMODE "RXPWM"	// 1 if the reciever output a pwm signal, else PPM
// SAVE/READ SETTINGS
// TYPE (value can be any number)
#define COMMAND_SAVE "SAVE"
#define COMMAND_READ "READ"
#define COMMAND_CONNECTED "CON"

typedef struct Command {
	char type[10];
	char value[10];
} Command;

Command decodeCommand(char *buffer);

void serialPrintString(uint8_t *s);
int modifySettingsWithCommand();
void loadSettingsFromEeprom();
void storeSettingsToEeprom();
void readSettingsFromEeprom();
uint8_t read_byte_eeprom(uint8_t address);
void write_byte_eeprom(uint8_t address, uint8_t value);
void serialPrintSettings();

#endif /* INC_SETTINGS_H_ */
