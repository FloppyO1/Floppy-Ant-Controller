/*
 * LSM6DS3.c
 *
 *  Created on: Mar 22, 2025
 *      Author: Filippo Castellan
 */

#include "stm32f0xx_hal.h"
#include "Libraries/LSM6DS3.h"

static const uint8_t DevAddress = 0xD4;	// 0x6A if SA0 = LOW, 0x6B if SA0 = HIGH


static uint8_t read_single_register(LSM6DS3 *LSM6DS3object, uint8_t regAddress);
static uint16_t read_two_consecutive_registers(LSM6DS3 *LSM6DS3object, uint8_t regfirstAddress);
static HAL_StatusTypeDef write_register (LSM6DS3 *LSM6DS3object, uint8_t regAddress, uint8_t data);

// FUNCTIONS FOR THE DRIVER
static uint8_t read_single_register(LSM6DS3 *LSM6DS3object, uint8_t regAddress) {
	uint8_t data = 0;
	HAL_I2C_Mem_Read(LSM6DS3object->i2c, DevAddress, regAddress, 1, &data, 1, TIMEOUT_I2C);
	return data;
}

static uint16_t read_two_consecutive_registers(LSM6DS3 *LSM6DS3object, uint8_t regfirstAddress) {
	uint8_t data[2];
	HAL_I2C_Mem_Read(LSM6DS3object->i2c, DevAddress, regfirstAddress, 1, data, 2, TIMEOUT_I2C);
	uint16_t returnData = (uint16_t) (data[1] << 8 | data[0]);
	return returnData;
}

static HAL_StatusTypeDef write_register(LSM6DS3 *LSM6DS3object, uint8_t regAddress, uint8_t data) {
	return HAL_I2C_Mem_Write(LSM6DS3object->i2c, DevAddress, regAddress, 1, &data, 1, TIMEOUT_I2C);
}

// FUNCTIONS

/*
 * @brief	Initialize the basic sensor
 * @note	try to communicate with him and reading a register
 *
 */
HAL_StatusTypeDef LSM6DS3_init(LSM6DS3 *LSM6DS3object, I2C_HandleTypeDef *hi2c) {
	HAL_Delay(15);	// wait 15ms to be sure that the sensor is fully on
	LSM6DS3object->i2c = hi2c;
	HAL_I2C_Init(LSM6DS3object->i2c);
	uint8_t data = read_single_register(LSM6DS3object, WHO_AM_I);
	if (data != WHO_AM_I_VALUE) {
		return HAL_ERROR;	// not the correct device responded to the i2c interface
	}
	uint8_t settings = 0b01000100;
	write_register(LSM6DS3object, CTRL3_C, settings);
	for (int i = 0; i < 3; i++)
		LSM6DS3object->gyro_offsets[i] = 0;
	return HAL_OK;	// initialization completed
}

HAL_StatusTypeDef LSM6DS3_init_accel(LSM6DS3 *LSM6DS3object) {
	HAL_StatusTypeDef stat;
	// init freq and scale accel
	uint8_t settings = 0b01100100;	// 0110 (416Hz), 01 (+-16g), 00 (100Hz bandwidth)
	stat = write_register(LSM6DS3object, CTRL1_XL, settings);
	HAL_Delay(20);
	return stat;
}

HAL_StatusTypeDef LSM6DS3_init_gyro(LSM6DS3 *LSM6DS3object) {
	HAL_StatusTypeDef stat;
	// init freq and scale accel
	uint8_t settings = 0b01101100;	// 0110 (416Hz), 11 (+-2000dps), 00 (not applicable)
	stat = write_register(LSM6DS3object, CTRL2_G, settings);
	HAL_Delay(20);
	return stat;
}

void LSM6DS3_update_accelerometer_all_values(LSM6DS3 *LSM6DS3object) {
	for (int i = 0; i < 3; i++)
		LSM6DS3_update_accelerometer_single_value(LSM6DS3object, i);
}

void LSM6DS3_update_accelerometer_single_value(LSM6DS3 *LSM6DS3object, uint8_t axis) {
	int16_t axis_xl = 0;
	switch (axis) {
		case X_AXIS:
			axis_xl = read_two_consecutive_registers(LSM6DS3object, OUTX_L_XL);
			LSM6DS3object->acc_x = (float) axis_xl * 0.488f / 1000.0f;	// convert the acceleration raw value in g
			break;
		case Y_AXIS:
			axis_xl = read_two_consecutive_registers(LSM6DS3object, OUTY_L_XL);
			LSM6DS3object->acc_y = (float) axis_xl * 0.488f / 1000.0f;	// convert the acceleration raw value in g
			break;
		case Z_AXIS:
			axis_xl = read_two_consecutive_registers(LSM6DS3object, OUTZ_L_XL);
			LSM6DS3object->acc_z = (float) axis_xl * 0.488f / 1000.0f;	// convert the acceleration raw value in g
			break;
	}
}

void LSM6DS3_update_gyroscope_all_values(LSM6DS3 *LSM6DS3object) {
	for (int i = 0; i < 3; i++)
		LSM6DS3_update_gyroscope_single_value(LSM6DS3object, i);
}
void LSM6DS3_update_gyroscope_single_value(LSM6DS3 *LSM6DS3object, uint8_t axis) {
	int16_t axis_g = 0;
	switch (axis) {
		case X_AXIS:
			axis_g = read_two_consecutive_registers(LSM6DS3object, OUTX_L_G) + LSM6DS3object->gyro_offsets[axis];
			LSM6DS3object->gyro_x = (float) axis_g * 0.07f;	// convert the acceleration raw value in deg/s
			break;
		case Y_AXIS:
			axis_g = read_two_consecutive_registers(LSM6DS3object, OUTY_L_G) + LSM6DS3object->gyro_offsets[axis];
			LSM6DS3object->gyro_y = (float) axis_g * 0.07f;	// convert the acceleration raw value in deg/s
			break;
		case Z_AXIS:
			axis_g = read_two_consecutive_registers(LSM6DS3object, OUTZ_L_G) + LSM6DS3object->gyro_offsets[axis];
			LSM6DS3object->gyro_z = (float) axis_g * 0.07f;	// convert the acceleration raw value in deg/s
			break;
	}
}

/*
 * @brief	Calculate new offset for gyro
 */
void LSM6DS3_calculate_offset(LSM6DS3 *LSM6DS3object) {
	int16_t avgOffsetX = 0;
	int16_t avgOffsetY = 0;
	int16_t avgOffsetZ = 0;
	for (int i = 0; i < 200; i++) {
		avgOffsetX = avgOffsetX + read_two_consecutive_registers(LSM6DS3object, OUTX_L_G);
		avgOffsetY = avgOffsetY + read_two_consecutive_registers(LSM6DS3object, OUTY_L_G);
		avgOffsetZ = avgOffsetZ + read_two_consecutive_registers(LSM6DS3object, OUTZ_L_G);
		avgOffsetX = avgOffsetX / 2;
		avgOffsetY = avgOffsetY / 2;
		avgOffsetZ = avgOffsetZ / 2;
	}
	LSM6DS3object->gyro_offsets[0] = -avgOffsetX;
	LSM6DS3object->gyro_offsets[1] = -avgOffsetY;
	LSM6DS3object->gyro_offsets[2] = -avgOffsetZ;
}

