/*
 * fac_eeprom.c
 *
 * All settings are stored in 16bit values, so in a 2kbit memory, 125 different settings can be stored
 *
 *  Created on: Aug 7, 2025
 *      Author: filippo-castellan
 */

#include "FAC_Code/fac_eeprom.h"
#include "main.h"
#include "i2c.h"

static Eeprom eeprom;

/* STATIC FUNCTION PROTORYPES */
static void FAC_eeprom_uint16_to_bytes(uint16_t value, uint8_t *array);
static uint16_t FAC_eeprom_bytes_to_uint16(const uint8_t *array);
static void FAC_eeprom_write_byte(uint8_t address, uint8_t value);
static uint8_t FAC_eeprom_read_byte(uint8_t address);
static void FAC_eeprom_SET_is_first_boot_value(uint8_t bootValue);

/* FUNCTION DEFINITION */
/* ----------------------PRIVATE FUNCTIONS---------------------- */
static void FAC_eeprom_SET_is_first_boot_value(uint8_t bootValue) {
	eeprom.is_first_boot_value = bootValue;

}

/**
 * @brief	Convert a uint16 into its two byte stored on a uint8_t array (second argoment)
 *
 */
static void FAC_eeprom_uint16_to_bytes(uint16_t value, uint8_t *array) {
	array[0] = (uint8_t) (value & 0xFF);        // LSB
	array[1] = (uint8_t) ((value >> 8) & 0xFF); // MSB
}

/**
 * @brief	Convert a uint8 array into the corrisponding uint16_t
 * @retval	Uint16_t converted form the byte array
 */
static uint16_t FAC_eeprom_bytes_to_uint16(const uint8_t *array) {
	return (uint16_t) array[0] | ((uint16_t) array[1] << 8);
}

/**
 * @brief 		Store a byte into the eeprom memory via i2c
 * @note		Only one byte at time can be stored
 * @datasheet	https://www.lcsc.com/datasheet/C7432363.pdf
 */
static void FAC_eeprom_write_byte(uint8_t address, uint8_t value) {
	uint8_t data = value;
	HAL_I2C_Mem_Write(&hi2c1, EEPROM_ADDRESS, address, 1, &data, 1, 1000);
	HAL_Delay(10);
}

/**
 * @brief 		Read a byte from the eeprom memory via i2c
 * @note		Only one byte at time can be read
 * @datasheet	https://www.lcsc.com/datasheet/C7432363.pdf
 */
static uint8_t FAC_eeprom_read_byte(uint8_t address) {
	uint8_t data;
	HAL_I2C_Mem_Read(&hi2c1, EEPROM_ADDRESS, address, 1, &data, 1, 1000);
	return data;
}

/* ----------------------PUBBLIC FUNCTIONS---------------------- */
uint8_t FAC_eeprom_GET_is_first_boot_value() {
	return eeprom.is_first_boot_value;
}

/**
 * @brief	Write to eeprom the "is first boot value", it is used to know if the eeprom is already initialized or not
 */
void FAC_eeprom_WRITE_frist_boot_value_in_eeprom(){
	FAC_eeprom_write_byte(EEPROM_ISFIRSTBOOT_ADDRESS, FAC_eeprom_GET_is_first_boot_value());
}

/**
 * @brief	Store into eeprom memory a two byte (uint16) value
 * @note	It consist in a multiple byte write operation, if the value is not changed don't do anything
 */
void FAC_eeprom_store_value(uint8_t position, uint16_t value) {
	if(FAC_eeprom_read_value(position) == value){
		return;
	}
	uint8_t array[2];
	FAC_eeprom_uint16_to_bytes(value, array);
	for (int i = 0; i < sizeof(uint16_t); i++) {
		FAC_eeprom_write_byte((position*2)+i, array[i]);
	}

}

/**
 * @brief	Read from eeprom memory a two byte (uint16) value
 * @note	It consist in a multiple byte read operation, *2 on the address is used because a setting value is made of 2 byte
 * @retval	Returns the value read from the eeprom in uint16 format
 */
uint16_t FAC_eeprom_read_value(uint8_t position) {
	uint8_t array[2];
	for (int i = 0; i < sizeof(uint16_t); i++) {
		array[i] = FAC_eeprom_read_byte((position*2) + i);
	}
	return FAC_eeprom_bytes_to_uint16(array);
}

/**
 * @brief 	This function read the eeprom byte that indicates if the settings are already been stored once.
 * @retval 	Return TRUE if it si the first time
 */
uint8_t FAC_eeprom_is_first_time() {
	uint8_t isFirst = FALSE;
	if (FAC_eeprom_read_byte(EEPROM_ISFIRSTBOOT_ADDRESS) != FAC_eeprom_GET_is_first_boot_value())
		isFirst = TRUE;
	return isFirst;
}

/**
 * @brief	Initialize the first boot value, if it is changed the settings are returned to the default value
 */
void FAC_eeprom_init(uint8_t bootValue) {
	if (bootValue == UINT8_MAX)	// this because the eeprom default value is 0xFF, so this value can not be used
		FAC_eeprom_SET_is_first_boot_value(bootValue - 1);
	else
		FAC_eeprom_SET_is_first_boot_value(bootValue);
}
