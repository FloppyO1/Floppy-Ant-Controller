/*
 * fac_gyro.h
 *
 *  Created on: Sep 17, 2025
 *      Author: Filippo Castellan
 */

#ifndef INC_FAC_CODE_FAC_IMU_H_
#define INC_FAC_CODE_FAC_IMU_H_

#include "stm32f0xx_hal.h"
#include "Libraries/LSM6DS3.h"

typedef struct Gyro {
	LSM6DS3 LSM6DS3object;
	HAL_StatusTypeDef gyro_status;	// HAL_OK, or HAL_ERROR	(in case of error the mix or function using it must manage the problem)
} Gyro;

HAL_StatusTypeDef FAC_IMU_GET_status();
float FAC_IMU_GET_accel_X();
float FAC_IMU_GET_accel_Y();
float FAC_IMU_GET_accel_Z();
float FAC_IMU_GET_gyro_X();
float FAC_IMU_GET_gyro_Y();
float FAC_IMU_GET_gyro_Z();
HAL_StatusTypeDef FAC_IMU_init();
void FAC_IMU_init_accelerometer();
void FAC_IMU_init_gyroscope();
void FAC_IMU_compute_gyro_offset();

#endif /* INC_FAC_CODE_FAC_IMU_H_ */
