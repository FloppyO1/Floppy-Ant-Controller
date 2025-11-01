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
#include "FAC_Code/fac_std_receiver.h"
#include "FAC_Code/fac_battery.h"
#include "FAC_Code/mixes_functions/fac_mixes.h"
#include "FAC_Code/config.h"
#include "FAC_Code/fac_app.h"
#include "FAC_Code/fac_imu.h"

uint8_t comSerialBuffer[64];			// buffer for received data from serial com

// @formatter:off
static Setting settings[FAC_SETTINGS_CODE_LAST] = {	// insert every single setting default values
/* { <NAME> , <VALUE> , <MIN> , <MAX> },
 * {SETTINGS_CODE_1, 10, 0, 100},
 * {SETTINGS_CODE_2, 6, 0, 50},
 * {SETTINGS_CODE_3, 5, 0, 250}freq
 */
/* MOTORS */
	{ FAC_SETTINGS_CODE_M1_REVERSED, FALSE, FALSE, TRUE },
	{ FAC_SETTINGS_CODE_M2_REVERSED, FALSE, FALSE, TRUE },
	{ FAC_SETTINGS_CODE_M3_REVERSED, FALSE, FALSE, TRUE },
	{ FAC_SETTINGS_CODE_M1_BRAKE_EN, FALSE, FALSE, TRUE },
	{ FAC_SETTINGS_CODE_M2_BRAKE_EN, FALSE, FALSE, TRUE },
	{ FAC_SETTINGS_CODE_M3_BRAKE_EN, FALSE, FALSE, TRUE },
	{ FAC_SETTINGS_CODE_MOTORS_FREQ, 500, 100, 10000 },			// min freq = 100 Hz, max freq = 10kHz
	/* SERVO */
	{ FAC_SETTINGS_CODE_S1_REVERSED, FALSE, FALSE, TRUE },
	{ FAC_SETTINGS_CODE_S2_REVERSED, FALSE, FALSE, TRUE },
	{ FAC_SETTINGS_CODE_S1_MIN_US_VALUE, 1000, 100, 1499 },			// min milliseconds value = 100us, max 1499us
	{ FAC_SETTINGS_CODE_S2_MIN_US_VALUE, 1000, 100, 1499 },
	{ FAC_SETTINGS_CODE_S1_MAX_US_VALUE, 2000, 1501, 2900 },		// min milliseconds value = 100us, max 1499us
	{ FAC_SETTINGS_CODE_S2_MAX_US_VALUE, 2000, 1501, 2900 },
	{ FAC_SETTINGS_CODE_SERVOS_FREQ, 50, 20, 300 },					// 20Hz min, 300 Hz max
	/* BATTERY */
	{ FAC_SETTINGS_CODE_LOW_BATTERY_VOLTAGE_MV, 3400, 2800, 4000 },	// min low battery 2800mV, max 4000mV
	{ FAC_SETTINGS_CODE_CUTOFF_VOLTAGE_MV, 2800, 0, 3500 },			// min cutoff 0mV (disabled), max 3500mV
	{ FAC_SETTINGS_CODE_CUTOFF_DETECTION_TIME, 10, 0, 30 },			// min cutoff detection time 0s (trigger instantaneously), max 30 seconds (trigger only if battery voltage stay below cutoff voltage for 30s)
	/* RECEIVER */
	{ FAC_SETTINGS_CODE_RECEIVER_TYPE, RECEIVER_TYPE_PPM, RECEIVER_TYPE_PWM, RECEIVER_TYPE_LAST-1 },
	{ FAC_SETTINGS_CODE_CHANNELS_DEADZONE_PERCENTAGE, 2, 0, 50 },	// expressed in percentage of the max receiver resolution
	{ FAC_SETTINGS_CODE_ARMING_CHANNEL, 5, 1, RECEIVER_CHANNELS_NUMBER },
	/* MIXES */
	{ FAC_SETTINGS_CODE_ACTIVE_MIX, FAC_MIX_SIMPLE_TANK, 0, FAC_MIX_LAST-1 },
	{ FAC_SETTINGS_CODE_MIX_INPUT1_CHANNEL, 2, 0, RECEIVER_CHANNELS_NUMBER },
	{ FAC_SETTINGS_CODE_MIX_INPUT2_CHANNEL, 1, 0, RECEIVER_CHANNELS_NUMBER },
	{ FAC_SETTINGS_CODE_MIX_INPUT3_CHANNEL, 0, 0, RECEIVER_CHANNELS_NUMBER },
	{ FAC_SETTINGS_CODE_MIX_INPUT4_CHANNEL, 0, 0, RECEIVER_CHANNELS_NUMBER },
	{ FAC_SETTINGS_CODE_MIX_INPUT5_CHANNEL, 0, 0, RECEIVER_CHANNELS_NUMBER },
	{ FAC_SETTINGS_CODE_MIX_INPUT6_CHANNEL, 0, 0, RECEIVER_CHANNELS_NUMBER },
	{ FAC_SETTINGS_CODE_MIX_INPUT7_CHANNEL, 0, 0, RECEIVER_CHANNELS_NUMBER },
	{ FAC_SETTINGS_CODE_MIX_INPUT8_CHANNEL, 0, 0, RECEIVER_CHANNELS_NUMBER },
	{ FAC_SETTINGS_CODE_MIX_INPUT1_REVERSED, FALSE, FALSE, TRUE },
	{ FAC_SETTINGS_CODE_MIX_INPUT2_REVERSED, FALSE, FALSE, TRUE },
	{ FAC_SETTINGS_CODE_MIX_INPUT3_REVERSED, FALSE, FALSE, TRUE },
	{ FAC_SETTINGS_CODE_MIX_INPUT4_REVERSED, FALSE, FALSE, TRUE },
	{ FAC_SETTINGS_CODE_MIX_INPUT5_REVERSED, FALSE, FALSE, TRUE },
	{ FAC_SETTINGS_CODE_MIX_INPUT6_REVERSED, FALSE, FALSE, TRUE },
	{ FAC_SETTINGS_CODE_MIX_INPUT7_REVERSED, FALSE, FALSE, TRUE },
	{ FAC_SETTINGS_CODE_MIX_INPUT8_REVERSED, FALSE, FALSE, TRUE },
	/* SPECIAL FUNCTIONS */
	{ FAC_SETTINGS_CODE_SPECIAL_FUNCTION1_INPUT_CHANNEL, 3, 0, RECEIVER_CHANNELS_NUMBER },	// 0 means function disabled
	{ FAC_SETTINGS_CODE_SPECIAL_FUNCTION2_INPUT_CHANNEL, 4, 0, RECEIVER_CHANNELS_NUMBER },
	{ FAC_SETTINGS_CODE_SPECIAL_FUNCTION3_INPUT_CHANNEL, 0, 0, RECEIVER_CHANNELS_NUMBER },
	{ FAC_SETTINGS_CODE_SPECIAL_FUNCTION4_INPUT_CHANNEL, 0, 0, RECEIVER_CHANNELS_NUMBER },
	{ FAC_SETTINGS_CODE_SPECIAL_FUNCTION5_INPUT_CHANNEL, 0, 0, RECEIVER_CHANNELS_NUMBER },
	{ FAC_SETTINGS_CODE_SPECIAL_FUNCTION6_INPUT_CHANNEL, 0, 0, RECEIVER_CHANNELS_NUMBER },
	{ FAC_SETTINGS_CODE_SPECIAL_FUNCTION7_INPUT_CHANNEL, 0, 0, RECEIVER_CHANNELS_NUMBER },
	{ FAC_SETTINGS_CODE_SPECIAL_FUNCTION8_INPUT_CHANNEL, 0, 0, RECEIVER_CHANNELS_NUMBER },
	{ FAC_SETTINGS_CODE_SPECIAL_FUNCTION9_INPUT_CHANNEL, 0, 0, RECEIVER_CHANNELS_NUMBER },
	{ FAC_SETTINGS_CODE_SPECIAL_FUNCTION10_INPUT_CHANNEL, 0, 0, RECEIVER_CHANNELS_NUMBER },
	{ FAC_SETTINGS_CODE_SPECIAL_FUNCTION11_INPUT_CHANNEL, 0, 0, RECEIVER_CHANNELS_NUMBER },
	{ FAC_SETTINGS_CODE_SPECIAL_FUNCTION12_INPUT_CHANNEL, 0, 0, RECEIVER_CHANNELS_NUMBER },
	{ FAC_SETTINGS_CODE_SPECIAL_FUNCTION13_INPUT_CHANNEL, 0, 0, RECEIVER_CHANNELS_NUMBER },
	{ FAC_SETTINGS_CODE_SPECIAL_FUNCTION14_INPUT_CHANNEL, 0, 0, RECEIVER_CHANNELS_NUMBER },
	{ FAC_SETTINGS_CODE_SPECIAL_FUNCTION15_INPUT_CHANNEL, 0, 0, RECEIVER_CHANNELS_NUMBER },
	{ FAC_SETTINGS_CODE_SPECIAL_FUNCTION16_INPUT_CHANNEL, 0, 0, RECEIVER_CHANNELS_NUMBER },
	{ FAC_SETTINGS_CODE_SPECIAL_FUNCTION17_INPUT_CHANNEL, 0, 0, RECEIVER_CHANNELS_NUMBER },
	{ FAC_SETTINGS_CODE_SPECIAL_FUNCTION18_INPUT_CHANNEL, 0, 0, RECEIVER_CHANNELS_NUMBER },
	{ FAC_SETTINGS_CODE_SPECIAL_FUNCTION19_INPUT_CHANNEL, 0, 0, RECEIVER_CHANNELS_NUMBER },
	{ FAC_SETTINGS_CODE_SPECIAL_FUNCTION20_INPUT_CHANNEL, 0, 0, RECEIVER_CHANNELS_NUMBER },
	/* MAPPER */
	/*
	 * Remember: 100 is mix prefix, 200 is spec. funct. prefix
	 * (ex: FAC_SETTINGS_CODE_MAPPER_M1 = 100+1 means that motor1
	 *  will have the value of the second (100+0 is the first)
	 *  output of the mix active)
	 */
	{ FAC_SETTINGS_CODE_MAPPER_M1, 100+0, 0, 200+10 },		// min is 100+0 output 0 of mix active, max is 200+10 output of the tenth special function
	{ FAC_SETTINGS_CODE_MAPPER_M2, 100+1, 0, 200+10 },		// 0 means it is not used
	{ FAC_SETTINGS_CODE_MAPPER_M3, 200+0, 0, 200+10 },
	{ FAC_SETTINGS_CODE_MAPPER_S1, 200+0, 0, 200+10 },
	{ FAC_SETTINGS_CODE_MAPPER_S2, 200+1, 0, 200+10 },

	/* FIRMWARE VERSION */
	{ FAC_SETTINGS_CODE_FIRMWARE_VERSION, 20000, 0, UINT16_MAX}	// 20513 -> 2.05.13 <major>,<minor>,<patch>
};
// @formatter:on

/* STATIC FUNCTION PROTORYPES */
static void FAC_settings_LOAD_ALL_from_eeprom();
static void FAC_settings_STORE_ALL_to_eeprom();
static void FAC_settings_SET_value(uint8_t code, uint16_t value);

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

/*
 * @biref	Load all settings form the eeprom
 * @note	Load directly to settings array
 */
static void FAC_settings_LOAD_ALL_from_eeprom() {
	for (int i = 0; i < FAC_SETTINGS_CODE_LAST; i++) {
		FAC_settings_SET_value(i, FAC_eeprom_read_value(i));
	}
}

/*
 * @biref	Store all settings to the eeprom
 * @note	Store the settings of the settings array
 */
static void FAC_settings_STORE_ALL_to_eeprom() {
	for (int i = 0; i < FAC_SETTINGS_CODE_LAST; i++) {
		FAC_eeprom_store_value(i, FAC_settings_GET_value(i));
	}
}

static void FAC_settings_STORE_new_firmware_version(uint8_t major, uint8_t minor, uint8_t patch) {
	uint16_t newVersion = (major * 10000) + (minor * 100) + patch;	// 20513 -> 2.05.13 <major>,<minor>,<patch>
	uint16_t currentVersion = FAC_settings_GET_value(FAC_SETTINGS_CODE_FIRMWARE_VERSION);
	if (newVersion != currentVersion) {
		FAC_eeprom_store_value(FAC_SETTINGS_CODE_FIRMWARE_VERSION, newVersion);
	}
}

/**
 * @brief		Convert a uint16 into its two byte stored on a uint8_t array (second argoment)
 * @note		MSB first (big endian)
 * @ATTENCTION	arm use little endian
 */
static void FAC_settings_uint16_to_bytes(uint16_t value, uint8_t *array) {
	array[1] = (uint8_t) (value & 0xFF);        // LSB
	array[0] = (uint8_t) ((value >> 8) & 0xFF); // MSB
}

/**
 * @brief		Convert a uint8 array into the corrisponding uint16_t
 * @retval		Uint16_t converted form the byte array
 * @note		MSB first (big endian)
 * @ATTENCTION	arm use little endian
 */
static uint16_t FAC_settings_bytes_to_uint16(const uint8_t *array) {
	return (uint16_t) array[0] | ((uint16_t) array[1] << 8);
}

/*
 * @brief	Send the telemetry via USB serial
 * @note	Telemetry message is composed by (21 bytes):
 * 			1B	Telemetry RES code											0
 * 			2B	ch1,2,3,4,5,6,7,8	(two byte each)							1-2, 3-4, 5-6, 7-8, 9-10, 11-12, 13-14, 15-16
 * 			2B	Vbat [mV]													17-18
 * 			1B	Battery type detected (0-5: USB, 1S, 2S, 3S, 4S, UNKNOWN)	19
 * 			1B	FAC state (0-2: DISARMED, NORMAL, CUTOFF)					20
 * 			2B	accel X	[mg]												21-22
 * 			2B	accel Y	[mg]												23-24
 * 			2B	accel Z	[mg]												25-26
 */
static void FAC_settings_USB_SEND_telemetry() {
	uint8_t telemetryPocket[27];
	/* telemetry res code */
	telemetryPocket[0] = FAC_USB_COMMAND_TELEMETRY_RESPONSE;
	/* receiver channels */
	for (int i = 0; i < 8; i++) {
		uint8_t ch[2];
		uint16_t chValue = FAC_std_receiver_GET_channel(i + 1);
		FAC_settings_uint16_to_bytes(chValue, ch);
		telemetryPocket[1 + (i * 2)] = ch[0];
		telemetryPocket[1 + ((i * 2) + 1)] = ch[1];
	}
	/* battery voltage */
	uint8_t vbat[2];
	FAC_settings_uint16_to_bytes(FAC_battery_GET_voltage(), vbat);
	telemetryPocket[17] = vbat[0];
	telemetryPocket[18] = vbat[1];
	/* battery type detected */			// 0-5: USB, 1S, 2S, 3S, 4S, UNKNOWN
	telemetryPocket[19] = FAC_battery_GET_type();
	/* fac state */					// 0-2: DISARMED, NORMAL, CUTOFF
	telemetryPocket[20] = FAC_app_GET_current_state();	// add the arming value
	/* accelerometer */
	uint8_t accel[2];
	// X
	int16_t accelTemp = (int16_t) (FAC_IMU_GET_accel_X() * 1000.0f);
	FAC_settings_uint16_to_bytes(accelTemp, accel);
	telemetryPocket[21] = accel[0];
	telemetryPocket[22] = accel[1];
	// Y
	accelTemp = (int16_t) (FAC_IMU_GET_accel_Y() * 1000.0f);
	FAC_settings_uint16_to_bytes(accelTemp, accel);
	telemetryPocket[23] = accel[0];
	telemetryPocket[24] = accel[1];
	// Z
	accelTemp = (int16_t) (FAC_IMU_GET_accel_Z() * 1000.0f);
	FAC_settings_uint16_to_bytes(accelTemp, accel);
	telemetryPocket[25] = accel[0];
	telemetryPocket[26] = accel[1];
	/* TRANMIT POCKET */
	CDC_Transmit_FS(telemetryPocket, sizeof(telemetryPocket));
}
/* ----------------------PUBBLIC FUNCTIONS---------------------- */
uint16_t FAC_settings_GET_value(uint8_t code) {
	return settings[code].value;
}

/*
 * @brief	understand the command received via COM serial and give the correct response
 * @retval	TRUE (1) if command understood, else FALSE (0)
 */
uint8_t FAC_settings_command_response() {
	uint8_t commandUndestood = FALSE;
	uint8_t command_code = comSerialBuffer[0];
	uint8_t setting_code = comSerialBuffer[1];
	/* understand the command received from usb serial communication */
	switch (command_code) {
		case FAC_USB_COMMAND_READ_VALUE:
			FAC_settings_USB_SEND_setting_value(setting_code);
			commandUndestood = TRUE;
			break;
		case FAC_USB_COMMAND_READ_RANGE:
			FAC_settings_USB_SEND_setting_ranges(setting_code);
			commandUndestood = TRUE;
			break;
		case FAC_USB_COMMAND_WRITE:
			uint8_t valueRaw[2] = {	// first 3 than 2 because data is sent in big endian (first msB than lsB) arm use little endian
				comSerialBuffer[3],
				comSerialBuffer[2] };
			uint16_t value = FAC_settings_bytes_to_uint16(valueRaw);
			FAC_settings_SET_value(setting_code, value);
			commandUndestood = TRUE;
			break;
		case FAC_USB_COMMAND_PING:
			uint8_t ping = 73;	// the perfect number XD (0x49)
			CDC_Transmit_FS(&ping, 1);
			commandUndestood = TRUE;
			break;
		case FAC_USB_COMMAND_TELEMETRY_REQUEST:
			FAC_settings_USB_SEND_telemetry();
			commandUndestood = TRUE;
			break;
		case FAC_USB_COMMAND_SAVE_TO_EEPROM:
			FAC_settings_STORE_ALL_to_eeprom();
			commandUndestood = TRUE;
			break;
		case FAC_USB_COMMAND_APPLY_SETTINGS:
			FAC_app_init_all_modules();	// reinitialize all the modules to apply the new settings
			commandUndestood = TRUE;
			break;
	}

	if (commandUndestood) {	// blink led if command understood, also add the usb_timeout time to the trasmition
//		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
//		HAL_Delay(USB_SERIAL_TIMEOUT);
//		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
	}
	return commandUndestood;
}

/**
 * @brief	Sends via COM serial the settings value
 * @note	Data format [code, valueLSB, valueMSB]
 */
void FAC_settings_USB_SEND_setting_value(uint8_t code) {
	uint8_t data[4];
	data[0] = FAC_USB_COMMAND_READ_VALUE;
	data[1] = code;
	/* min */
	data[3] = (uint8_t) (settings[code].value & 0xFF);        // LSB
	data[2] = (uint8_t) ((settings[code].value >> 8) & 0xFF); // MSB

	CDC_Transmit_FS(data, sizeof(data));
}

/**
 * @brief	Sends via COM serial the settings value ranges
 * @note	Data format [code, minLSB, minMSB, maxLSB, maxMSB]
 */
void FAC_settings_USB_SEND_setting_ranges(uint8_t code) {
	uint8_t data[6];
	data[0] = FAC_USB_COMMAND_READ_RANGE;
	data[1] = code;
	/* min */
	data[3] = (uint8_t) (settings[code].min_value & 0xFF);        // LSB
	data[2] = (uint8_t) ((settings[code].min_value >> 8) & 0xFF); // MSB
	/* max */
	data[5] = (uint8_t) (settings[code].max_value & 0xFF);        // LSB
	data[4] = (uint8_t) ((settings[code].max_value >> 8) & 0xFF); // MSB
	CDC_Transmit_FS(data, sizeof(data));
}

void FAC_settings_init(uint8_t bootValue) {
	/* FIRMWARE VERSION UPDATE */
	FAC_settings_STORE_new_firmware_version(FIRMWARE_VERSION_MAJOR, FIRMWARE_VERSION_MINOR, FIRMWARE_VERSION_PATCH);

	/* SETTINGS LOAD */
	FAC_eeprom_init(bootValue);		// set the "first boot" value
	if (FAC_eeprom_is_first_time()) {	// if the eeprom doesn´t contain any settings yet
		// STORE TO THE DEFAULT SETTINGS TO THE EEPROM
		FAC_settings_STORE_ALL_to_eeprom();
		/* A LOTS OF BLINK TO INDICARTE AN MASSIVE EEPROM WRITE */
		for (int i = 0; i < 10; i++) {
			HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
			HAL_Delay(50);
			HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
			HAL_Delay(50);
		}

		FAC_eeprom_WRITE_frist_boot_value_in_eeprom();	// store the new bootValue
	} else {	// if the eeprom already contains settings
		// LOAD FROM EEPROM SAVED SETTINGS
		FAC_settings_LOAD_ALL_from_eeprom();

		/* SOME BLINKS TO INDICATE A NORMAL EEPROM READ */
		for (int i = 0; i < 3; i++) {
			HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
			HAL_Delay(50);
			HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
			HAL_Delay(50);
		}
	}
}

/*
 * @brief	Sends via USB COM wdatahat the USB serial COM received
 * @note	Send the whole USB serial buffer
 */
void FAC_settings_SEND_what_received() {
	CDC_Transmit_FS(comSerialBuffer, sizeof(comSerialBuffer));
}
