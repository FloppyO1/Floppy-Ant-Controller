/*
 * fac_settings.c
 *
 *  Created on: Aug 8, 2025
 *      Author: filippo-castellan
 */

#include "FAC_Code/fac_settings.h"
//#include "usb_device.h"
#include "usbd_cdc_if.h"
#include "FAC_Code/fac_eeprom.h"

uint8_t comSerialBuffer[64];			// buffer for received data from serial com

static Setting settings[FAC_SETTINGS_CODE_LAST] = {	// insert every single setting default values
/* {SETTINGS_CODE_1, 10, 0, 100},
 * {SETTINGS_CODE_2, 6, 0, 50},
 * {SETTINGS_CODE_3, 5, 0, 250}
 */
{ FAC_SETTINGS_CODE_CUTOFF, 354, 0, 1000 } };

/* STATIC FUNCTION PROTORYPES */
static void FAC_settings_LOAD_ALL_from_eeprom();
static void FAC_settings_STORE_ALL_to_eeprom();
static void FAC_settings_SET_value(uint8_t code, uint16_t value);
static uint16_t FAC_settings_GET_value(uint8_t code);

/* FUNCTION DEFINITION */
/* ----------------------PRIVATE FUNCTIONS---------------------- */
static void FAC_settings_SET_value(uint8_t code, uint16_t value) {
	uint16_t v = value;
	/* check if value is inside the range */
	if (v < settings[code].min_value)
		v = settings[code].min_value;
	if (v > settings[code].max_value)
		v = settings[code].max_value;
	/* store to the settings array */
	settings[code].value = v;
}

static uint16_t FAC_settings_GET_value(uint8_t code) {
	return settings[code].value;
}

/*
 * @biref	Load all settings form the eeprom
 * @note	Load directly to settings array
 */
static void FAC_settings_LOAD_ALL_from_eeprom() {
	for (int i = 0; i < FAC_SETTINGS_CODE_LAST; i++) {
		settings[i].value = FAC_eeprom_read_value(i);
	}
}

/*
 * @biref	Store all settings to the eeprom
 * @note	Store the settings of the settings array
 */
static void FAC_settings_STORE_ALL_to_eeprom() {
	for (int i = 0; i < FAC_SETTINGS_CODE_LAST; i++) {
		FAC_eeprom_store_value(i, settings[i].value);
	}
}
/* ----------------------PUBBLIC FUNCTIONS---------------------- */
/**
 * @brief	Sends via COM serial the settings value
 * @note	Data format [code, valueLSB, valueMSB]
 */
void FAC_settings_USB_SEND_setting_value(uint8_t code) {
	uint8_t data[3];
	data[0] = code;
	/* value */
	data[1] = (uint8_t) (settings[code].value & 0xFF);        // LSB
	data[2] = (uint8_t) ((settings[code].value >> 8) & 0xFF); // MSB

	CDC_Transmit_FS(data, sizeof(data));
}

/**
 * @brief	Sends via COM serial the settings value ranges
 * @note	Data format [code, minLSB, minMSB, maxLSB, maxMSB]
 */
void FAC_settings_USB_SEND_setting_ranges(uint8_t code) {
	uint8_t data[5];
	data[0] = code;
	/* min */
	data[1] = (uint8_t) (settings[code].min_value & 0xFF);        // LSB
	data[2] = (uint8_t) ((settings[code].min_value >> 8) & 0xFF); // MSB
	/* max */
	data[3] = (uint8_t) (settings[code].max_value & 0xFF);        // LSB
	data[4] = (uint8_t) ((settings[code].max_value >> 8) & 0xFF); // MSB
	CDC_Transmit_FS(data, sizeof(data));
}

void FAC_settings_init(uint8_t bootValue) {
	FAC_eeprom_init(1);
	if (FAC_eeprom_GET_is_first_boot_value()) {	// if the eeprom doesn´t contain any settings yet
		// STORE TO THE DEFAULT SETTINGS TO THE EEPROM
		FAC_eeprom_WRITE_frist_boot_value_in_eeprom(bootValue);	// store the new bootValue
	} else {	// if the eeprom already contains settings
		// LOAD FROM EEPROM SAVED SETTINGS
	}
}
