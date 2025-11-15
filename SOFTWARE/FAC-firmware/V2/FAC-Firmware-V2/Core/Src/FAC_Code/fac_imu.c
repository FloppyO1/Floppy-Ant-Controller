/*
 * fac_gyro.c
 *
 *  Created on: Sep 17, 2025
 *      Author: Filippo Castellan
 */

#include "FAC_Code/fac_imu.h"
#include "i2c.h"

static Gyro gyro;

/* STATIC FUNCTION PROTORYPES */

/* STATIC FUNCTION */
static void FAC_IMU_SET_status(HAL_StatusTypeDef gyro_status){
	gyro.gyro_status = gyro_status;
}

static LSM6DS3* FAC_IMU_GET_LSM6DS3_object(){
	return &gyro.LSM6DS3object;
}

/* FUNCTION DEFINITION */

HAL_StatusTypeDef FAC_IMU_GET_status(){
	return gyro.gyro_status;
}

float FAC_IMU_GET_accel_X(){
	LSM6DS3_update_accelerometer_single_value(FAC_IMU_GET_LSM6DS3_object(), X_AXIS);
	return FAC_IMU_GET_LSM6DS3_object()->acc_x;
}

float FAC_IMU_GET_accel_Y(){
	LSM6DS3_update_accelerometer_single_value(FAC_IMU_GET_LSM6DS3_object(), Y_AXIS);
	return FAC_IMU_GET_LSM6DS3_object()->acc_y;
}

float FAC_IMU_GET_accel_Z(){
	LSM6DS3_update_accelerometer_single_value(FAC_IMU_GET_LSM6DS3_object(), Z_AXIS);
	return FAC_IMU_GET_LSM6DS3_object()->acc_z;
}

float FAC_IMU_GET_gyro_X(){
	LSM6DS3_update_gyroscope_single_value(FAC_IMU_GET_LSM6DS3_object(), X_AXIS);
	return FAC_IMU_GET_LSM6DS3_object()->gyro_x;
}

float FAC_IMU_GET_gyro_Y(){
	LSM6DS3_update_gyroscope_single_value(FAC_IMU_GET_LSM6DS3_object(), Y_AXIS);
	return FAC_IMU_GET_LSM6DS3_object()->gyro_y;
}

float FAC_IMU_GET_gyro_Z(){
	LSM6DS3_update_gyroscope_single_value(FAC_IMU_GET_LSM6DS3_object(), Z_AXIS);
	return FAC_IMU_GET_LSM6DS3_object()->gyro_z;
}

/*
 * @brief	Init the accelerometer of the chip
 */
void FAC_IMU_init_accelerometer(){
	if(FAC_IMU_GET_status() != HAL_ERROR){
		LSM6DS3_init_accel(FAC_IMU_GET_LSM6DS3_object());
		HAL_Delay(100);
	}
}

/*
 * @brief	Init the gyroscope of the chip
 */
void FAC_IMU_init_gyroscope(){
	if(FAC_IMU_GET_status() != HAL_ERROR){
		LSM6DS3_init_gyro(FAC_IMU_GET_LSM6DS3_object());
		HAL_Delay(100);
	}
}

/*
 * @brief	Compute new gyro offsets
 */
void FAC_IMU_compute_gyro_offset(){
	HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
	LSM6DS3_calculate_offset(FAC_IMU_GET_LSM6DS3_object());
	HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
}

/*
 * @brief		Initialize the gyro/accel LSM6DS3
 * @IMPORTANT	If status is HAL_ERROR the mix or special function must manage this problem
 */
void FAC_IMU_init(){
	HAL_Delay(200);	// wait for the imu startup
	HAL_StatusTypeDef gyro_status = LSM6DS3_init(FAC_IMU_GET_LSM6DS3_object(), &hi2c1);
	if(gyro_status == HAL_ERROR)
		FAC_IMU_SET_status(gyro_status);
}
