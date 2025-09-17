/*
 * LSM6DS3.h
 *
 *  Created on: Mar 22, 2025
 *      Author: Filippo Castellan
 */

#ifndef INC_MYCODE_LSM6DS3_H_
#define INC_MYCODE_LSM6DS3_H_

#define READ 1
#define WRITE 0

#include "stm32f0xx_hal.h"

// REGISTERS
#define FUNC_CFG_ACCESS     0x01  // Embedded functions configuration register
#define SENSOR_SYNC_TIME_FRAME 0x04  // Sensor sync configuration register
#define SENSOR_SYNC_RES_RATIO  0x05  // Sensor sync resolution ratio register
#define FIFO_CTRL1          0x06  // FIFO configuration register 1
#define FIFO_CTRL2          0x07  // FIFO configuration register 2
#define FIFO_CTRL3          0x08  // FIFO configuration register 3
#define FIFO_CTRL4          0x09  // FIFO configuration register 4
#define FIFO_CTRL5          0x0A  // FIFO configuration register 5
#define DRDY_PULSE_CFG_G    0x0B  // Data-ready pulse configuration register
#define INT1_CTRL           0x0D  // INT1 pin control
#define INT2_CTRL           0x0E  // INT2 pin control
#define WHO_AM_I            0x0F  // Who I am ID
#define CTRL1_XL            0x10  // Accelerometer control
#define CTRL2_G             0x11  // Gyroscope control
#define CTRL3_C             0x12  // Common control register
#define CTRL4_C             0x13  // Common control register
#define CTRL5_C             0x14  // Common control register
#define CTRL6_C             0x15  // Common control register
#define CTRL7_G             0x16  // Gyroscope control
#define CTRL8_XL            0x17  // Accelerometer control
#define CTRL9_XL            0x18  // Accelerometer control
#define CTRL10_C            0x19  // Common control register
#define MASTER_CONFIG       0x1A  // I2C master configuration register
#define WAKE_UP_SRC         0x1B  // Wake-up source register
#define TAP_SRC             0x1C  // Tap source register
#define D6D_SRC             0x1D  // 6D source register
#define STATUS_REG          0x1E  // Status register
#define OUT_TEMP_L          0x20  // Temperature output LSB
#define OUT_TEMP_H          0x21  // Temperature output MSB
#define OUTX_L_G            0x22  // Angular rate X-axis output LSB
#define OUTX_H_G            0x23  // Angular rate X-axis output MSB
#define OUTY_L_G            0x24  // Angular rate Y-axis output LSB
#define OUTY_H_G            0x25  // Angular rate Y-axis output MSB
#define OUTZ_L_G            0x26  // Angular rate Z-axis output LSB
#define OUTZ_H_G            0x27  // Angular rate Z-axis output MSB
#define OUTX_L_XL           0x28  // Acceleration X-axis output LSB
#define OUTX_H_XL           0x29  // Acceleration X-axis output MSB
#define OUTY_L_XL           0x2A  // Acceleration Y-axis output LSB
#define OUTY_H_XL           0x2B  // Acceleration Y-axis output MSB
#define OUTZ_L_XL           0x2C  // Acceleration Z-axis output LSB
#define OUTZ_H_XL           0x2D  // Acceleration Z-axis output MSB
#define SENSORHUB1_REG      0x2E  // Sensor hub output register 1
#define SENSORHUB2_REG      0x2F  // Sensor hub output register 2
#define SENSORHUB3_REG      0x30  // Sensor hub output register 3
#define SENSORHUB4_REG      0x31  // Sensor hub output register 4
#define SENSORHUB5_REG      0x32  // Sensor hub output register 5
#define SENSORHUB6_REG      0x33  // Sensor hub output register 6
#define SENSORHUB7_REG      0x34  // Sensor hub output register 7
#define SENSORHUB8_REG      0x35  // Sensor hub output register 8
#define SENSORHUB9_REG      0x36  // Sensor hub output register 9
#define SENSORHUB10_REG     0x37  // Sensor hub output register 10
#define SENSORHUB11_REG     0x38  // Sensor hub output register 11
#define SENSORHUB12_REG     0x39  // Sensor hub output register 12
#define FIFO_STATUS1        0x3A  // FIFO status register 1
#define FIFO_STATUS2        0x3B  // FIFO status register 2
#define FIFO_STATUS3        0x3C  // FIFO status register 3
#define FIFO_STATUS4        0x3D  // FIFO status register 4
#define FIFO_DATA_OUT_L     0x3E  // FIFO data output LSB
#define FIFO_DATA_OUT_H     0x3F  // FIFO data output MSB
#define TIMESTAMP0_REG      0x40  // Timestamp output register 0
#define TIMESTAMP1_REG      0x41  // Timestamp output register 1
#define TIMESTAMP2_REG      0x42  // Timestamp output register 2
#define STEP_TIMESTAMP_L    0x49  // Step counter timestamp LSB
#define STEP_TIMESTAMP_H    0x4A  // Step counter timestamp MSB
#define STEP_COUNTER_L      0x4B  // Step counter output LSB
#define STEP_COUNTER_H      0x4C  // Step counter output MSB
#define SENSORHUB13_REG     0x4D  // Sensor hub output register 13
#define SENSORHUB14_REG     0x4E  // Sensor hub output register 14
#define SENSORHUB15_REG     0x4F  // Sensor hub output register 15
#define SENSORHUB16_REG     0x50  // Sensor hub output register 16
#define SENSORHUB17_REG     0x51  // Sensor hub output register 17
#define SENSORHUB18_REG     0x52  // Sensor hub output register 18
#define FUNC_SRC1           0x53  // Function source register 1
#define FUNC_SRC2           0x54  // Function source register 2
#define WRIST_TILT_IA       0x55  // Wrist tilt interrupt register
#define TAP_CFG             0x58  // Tap configuration
#define TAP_THS_6D          0x59  // Tap threshold for 6D
#define INT_DUR2            0x5A  // Tap duration
#define WAKE_UP_THS         0x5B  // Wake-up threshold
#define WAKE_UP_DUR         0x5C  // Wake-up duration
#define FREE_FALL           0x5D  // Free-fall configuration
#define MD1_CFG             0x5E  // MD1 configuration
#define MD2_CFG             0x5F  // MD2 configuration
#define MASTER_CMD_CODE     0x60  // Master command code
#define SENS_SYNC_SPI_ERROR_CODE 0x61  // SPI error code register
#define OUT_MAG_RAW_X_L     0x66  // External magnetometer X-axis output LSB
#define OUT_MAG_RAW_X_H     0x67  // External magnetometer X-axis output MSB
#define OUT_MAG_RAW_Y_L     0x68  // External magnetometer Y-axis output LSB
#define OUT_MAG_RAW_Y_H     0x69  // External magnetometer Y-axis output MSB
#define OUT_MAG_RAW_Z_L     0x6A  // External magnetometer Z-axis output LSB
#define OUT_MAG_RAW_Z_H     0x6B  // External magnetometer Z-axis output MSB
#define X_OFS_USR           0x73  // Accelerometer X-axis user offset
#define Y_OFS_USR           0x74  // Accelerometer Y-axis user offset
#define Z_OFS_USR           0x75  // Accelerometer Z-axis user offset

// KNOWN VALUE
//#define WHO_AM_I_VALUE 		0x6A	// see page 60 of the LSM6DS3 datasheet
#define WHO_AM_I_VALUE 		0x69	// a Chinese clone gives me 0x69 ???

// SETTINGS
#define TIMEOUT_I2C	100

// STRUCT
typedef struct LSM6DS3{
	I2C_HandleTypeDef *i2c;
	float acc_x;
	float acc_y;
	float acc_z;
	float gyro_x;
	float gyro_y;
	float gyro_z;
	int16_t gyro_offsets[];	// x, y, z
}LSM6DS3;

enum AXIS{
	X_AXIS,
	Y_AXIS,
	Z_AXIS
};

// FUNCTIONS PROTOTYPES
HAL_StatusTypeDef LSM6DS3_init(LSM6DS3 *LSM6DS3object, I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef LSM6DS3_init_accel(LSM6DS3 *LSM6DS3object);
HAL_StatusTypeDef LSM6DS3_init_gyro(LSM6DS3 *LSM6DS3object);
void LSM6DS3_update_accelerometer_all_values(LSM6DS3 *LSM6DS3object);
void LSM6DS3_update_accelerometer_single_value(LSM6DS3 *LSM6DS3object, uint8_t axis);
void LSM6DS3_update_gyroscope_single_value(LSM6DS3 *LSM6DS3object, uint8_t axis);
void LSM6DS3_update_gyroscope_all_values(LSM6DS3 *LSM6DS3object);
void LSM6DS3_calculate_offset(LSM6DS3 *LSM6DS3object);

#endif /* INC_MYCODE_LSM6DS3_H_ */
