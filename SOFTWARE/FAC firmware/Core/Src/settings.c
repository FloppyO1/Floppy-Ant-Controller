/*
 * settings.c
 *
 *  Created on: Jul 14, 2024
 *      Author: Filippo Castellan
 */

#include "settings.h"
#include "usbd_cdc_if.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// DEFINEs & OBJECTs & VARIABLES FOR EEPROM
#define EEPROM_ADDRESS 0xA0	//devide address
extern I2C_HandleTypeDef hi2c1;

#define SETTINGS_NUMBER 20

extern uint8_t thChannel;	// defined inside main.c
extern uint8_t stChannel;
extern uint8_t wpChannel;
extern uint8_t s1Channel;
extern uint8_t s2Channel;
extern uint8_t armChannel;

extern uint8_t m1Rev;
extern uint8_t m2Rev;
extern uint8_t m3Rev;
extern uint8_t s1Rev;
extern uint8_t s2Rev;

extern uint8_t mLeft;
extern uint8_t mRight;
extern uint8_t mWeapon;

extern uint8_t weaponDoubleDirection;
extern uint8_t tankMixIsON;

extern uint8_t noDisarm;

extern uint8_t limitVoltage;
extern uint8_t cutOffVoltage;

extern uint8_t recieverIsPWM;

// USB THINGS
char buffer[64];	//buffer from the USB (serial) incoming data

int modifySettingsWithCommand() {
	uint8_t isOK = FALSE;
	Command c = decodeCommand(buffer);
	//find the command
	// CHANNELs MAPPING
	if (strcmp(c.type, COMMAND_TH2CH) == 0) {	// COMMAND_TH2CH recieved
		thChannel = atoi(c.value);	// convert value string into integer value (the value of the channel)
		isOK = TRUE;
	}
	if (strcmp(c.type, COMMAND_ST2CH) == 0) {
		stChannel = atoi(c.value);
		isOK = TRUE;
	}
	if (strcmp(c.type, COMMAND_WP2CH) == 0) {
		wpChannel = atoi(c.value);
		isOK = TRUE;
	}
	if (strcmp(c.type, COMMAND_S12CH) == 0) {
		s1Channel = atoi(c.value);
		isOK = TRUE;
	}
	if (strcmp(c.type, COMMAND_S22CH) == 0) {
		s2Channel = atoi(c.value);
		isOK = TRUE;
	}
	if (strcmp(c.type, COMMAND_ARM) == 0) {
		armChannel = atoi(c.value);
		isOK = TRUE;
	}
	// REVERSE MOTORs & SERVOs
	if (strcmp(c.type, COMMAND_M1REV) == 0) {
		m1Rev = atoi(c.value);
		isOK = TRUE;
	}
	if (strcmp(c.type, COMMAND_M2REV) == 0) {
		m2Rev = atoi(c.value);
		isOK = TRUE;
	}
	if (strcmp(c.type, COMMAND_M3REV) == 0) {
		m3Rev = atoi(c.value);
		isOK = TRUE;
	}
	if (strcmp(c.type, COMMAND_S1REV) == 0) {
		s1Rev = atoi(c.value);
		isOK = TRUE;
	}
	if (strcmp(c.type, COMMAND_S2REV) == 0) {
		s2Rev = atoi(c.value);
		isOK = TRUE;
	}
	// MOTORs ASSIGNMENT
	if (strcmp(c.type, COMMAND_M1ASS) == 0) {
		if (strcmp(c.value, VALUE_ML) == 0) {
			mLeft = M1;
			isOK = TRUE;
		}
		if (strcmp(c.value, VALUE_MR) == 0) {
			mRight = M1;
			isOK = TRUE;
		}
		if (strcmp(c.value, VALUE_WP) == 0) {
			mWeapon = M1;
			isOK = TRUE;
		}
	}
	if (strcmp(c.type, COMMAND_M2ASS) == 0) {
		if (strcmp(c.value, VALUE_ML) == 0) {
			mLeft = M2;
			isOK = TRUE;
		}
		if (strcmp(c.value, VALUE_MR) == 0) {
			mRight = M2;
			isOK = TRUE;
		}
		if (strcmp(c.value, VALUE_WP) == 0) {
			mWeapon = M2;
			isOK = TRUE;
		}
	}
	if (strcmp(c.type, COMMAND_M3ASS) == 0) {
		if (strcmp(c.value, VALUE_ML) == 0) {
			mLeft = M3;
			isOK = TRUE;
		}
		if (strcmp(c.value, VALUE_MR) == 0) {
			mRight = M3;
			isOK = TRUE;
		}
		if (strcmp(c.value, VALUE_WP) == 0) {
			mWeapon = M3;
			isOK = TRUE;
		}
	}
	// WEAPPON DOUBLE DIRECTION
	if (strcmp(c.type, COMMAND_WPDD) == 0) {
		weaponDoubleDirection = atoi(c.value);
		isOK = TRUE;
	}
	// TANK MIX
	if (strcmp(c.type, COMMAND_TANKON) == 0) {
		tankMixIsON = atoi(c.value);
		isOK = TRUE;
	}
	// OTHER
	if (strcmp(c.type, COMMAND_NODISARM) == 0) {
		noDisarm = atoi(c.value);
		isOK = TRUE;
	}
	if (strcmp(c.type, COMMAND_LIMITVOLTAGE) == 0) {
		limitVoltage = atoi(c.value);
		isOK = TRUE;
	}
	if (strcmp(c.type, COMMAND_CUTOFFVOLTAGE) == 0) {
		cutOffVoltage = atoi(c.value);
		isOK = TRUE;
	}
	if (strcmp(c.type, COMMAND_RXPWMMODE) == 0) {
		recieverIsPWM = atoi(c.value);
		isOK = TRUE;
	}

	// SAVE AND READ
	if (strcmp(c.type, COMMAND_SAVE) == 0) {
		storeSettingsToEeprom();
		isOK = TRUE;
	}
	if (strcmp(c.type, COMMAND_READ) == 0) {
		readSettingsFromEeprom();
		serialPrintSettings();
		isOK = TRUE;
	}
	return isOK;
}

Command decodeCommand(char *buffer) {
	Command c;
	// make a copy of the buffer and use it because strtok is destructive
	char temp[strlen(buffer)];
	strcpy(temp, buffer);

	char *token = strtok(temp, COMMAND_TYPE_SEPARATOR);
	strcpy(c.type, token);
	token = strtok(NULL, COMMAND_TYPE_SEPARATOR);
	strcpy(c.value, token);
	return c;
}

void serialPrintString(uint8_t *s) {
	CDC_Transmit_FS(s, strlen(s));
}

void storeSettingsToEeprom() {
	// prepare data to write
	uint8_t dataToStore[SETTINGS_NUMBER];

	dataToStore[0] = thChannel;
	dataToStore[1] = stChannel;
	dataToStore[2] = wpChannel;
	dataToStore[3] = s1Channel;
	dataToStore[4] = s2Channel;
	dataToStore[5] = armChannel;

	dataToStore[6] = m1Rev;
	dataToStore[7] = m2Rev;
	dataToStore[8] = m3Rev;
	dataToStore[9] = s1Rev;
	dataToStore[10] = s2Rev;

	dataToStore[11] = mLeft;
	dataToStore[12] = mRight;
	dataToStore[13] = mWeapon;

	dataToStore[14] = weaponDoubleDirection;
	dataToStore[15] = tankMixIsON;

	dataToStore[16] = noDisarm;
	dataToStore[17] = limitVoltage;
	dataToStore[18] = cutOffVoltage;
	dataToStore[19] = recieverIsPWM;
	// write to memory
	for (int i = EEPROM_DATA_BEGIN_ADDRESS; i < SETTINGS_NUMBER; i++) {
		write_byte_eeprom(i, dataToStore[i]);
	}
}

void readSettingsFromEeprom() {
	uint8_t dataRead[SETTINGS_NUMBER];
	for (int i = EEPROM_DATA_BEGIN_ADDRESS; i < SETTINGS_NUMBER; i++) {
		dataRead[i] = read_byte_eeprom(i);
	}

	thChannel = dataRead[0];
	stChannel = dataRead[1];
	wpChannel = dataRead[2];
	s1Channel = dataRead[3];
	s2Channel = dataRead[4];
	armChannel = dataRead[5];

	m1Rev = dataRead[6];
	m2Rev = dataRead[7];
	m3Rev = dataRead[8];
	s1Rev = dataRead[9];
	s2Rev = dataRead[10];

	mLeft = dataRead[11];
	mRight = dataRead[12];
	mWeapon = dataRead[13];

	weaponDoubleDirection = dataRead[14];
	tankMixIsON = dataRead[15];

	noDisarm = dataRead[16];
	limitVoltage = dataRead[17];
	cutOffVoltage = dataRead[18];
	recieverIsPWM = dataRead[19];
}

void loadSettingsFromEeprom() {
	readSettingsFromEeprom();
}

uint8_t read_byte_eeprom(uint8_t address) {
	uint8_t data;
	HAL_I2C_Mem_Read(&hi2c1, EEPROM_ADDRESS, address, 1, &data, 1, 1000);
	return data;
}

void write_byte_eeprom(uint8_t address, uint8_t value) {
	uint8_t data = value;
	HAL_I2C_Mem_Write(&hi2c1, EEPROM_ADDRESS, address, 1, &data, 1, 1000);
	HAL_Delay(10);
}

void serialPrintSettings() {
	uint8_t dataRead[SETTINGS_NUMBER];
	dataRead[0] = thChannel;
	dataRead[1] = stChannel;
	dataRead[2] = wpChannel;
	dataRead[3] = s1Channel;
	dataRead[4] = s2Channel;
	dataRead[5] = armChannel;

	dataRead[6] = m1Rev;
	dataRead[7] = m2Rev;
	dataRead[8] = m3Rev;
	dataRead[9] = s1Rev;
	dataRead[10] = s2Rev;

	dataRead[11] = mLeft;
	dataRead[12] = mRight;
	dataRead[13] = mWeapon;

	dataRead[14] = weaponDoubleDirection;
	dataRead[15] = tankMixIsON;

	dataRead[16] = noDisarm;
	dataRead[17] = limitVoltage;
	dataRead[18] = cutOffVoltage;
	dataRead[19] = recieverIsPWM;

	const uint8_t *commands[] = { (const uint8_t*) COMMAND_TH2CH, (const uint8_t*) COMMAND_ST2CH,
			(const uint8_t*) COMMAND_WP2CH, (const uint8_t*) COMMAND_S12CH, (const uint8_t*) COMMAND_S22CH,
			(const uint8_t*) COMMAND_ARM, (const uint8_t*) COMMAND_M1REV, (const uint8_t*) COMMAND_M2REV,
			(const uint8_t*) COMMAND_M3REV, (const uint8_t*) COMMAND_S1REV, (const uint8_t*) COMMAND_S2REV,
			(const uint8_t*) COMMAND_M1ASS, (const uint8_t*) COMMAND_M2ASS, (const uint8_t*) COMMAND_M3ASS,
			(const uint8_t*) COMMAND_WPDD, (const uint8_t*) COMMAND_TANKON, (const uint8_t*) COMMAND_NODISARM,
			(const uint8_t*) COMMAND_LIMITVOLTAGE, (const uint8_t*) COMMAND_CUTOFFVOLTAGE, (const uint8_t*) COMMAND_RXPWMMODE };

	uint8_t c[] = "\nSETTINGS:\n";
	serialPrintString(c);
	HAL_Delay(0);
	for (int i = 0; i < SETTINGS_NUMBER; i++) {
		uint8_t string[30];
		sprintf(string, "%s = %d\n", commands[i], dataRead[i]);
		serialPrintString(string);
		HAL_Delay(10);
	}
}
