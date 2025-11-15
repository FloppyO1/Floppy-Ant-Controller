/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdlib.h"
#include "reciever.h"
#include "servo.h"
#include "motor.h"
#include "settings.h"
#include "battery.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
int16_t map(int16_t x, int16_t in_min, int16_t in_max, int16_t out_min, int16_t out_max);
uint8_t checkChannelOnCenter(uint8_t channel);
uint8_t checkChannelAtZero(uint8_t channel);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define U_LED_Pin GPIO_PIN_15
#define U_LED_GPIO_Port GPIOC
#define VBAT_Pin GPIO_PIN_0
#define VBAT_GPIO_Port GPIOA
#define POT1_Pin GPIO_PIN_1
#define POT1_GPIO_Port GPIOA
#define POT2_Pin GPIO_PIN_2
#define POT2_GPIO_Port GPIOA
#define CH1_Pin GPIO_PIN_0
#define CH1_GPIO_Port GPIOB
#define CH1_EXTI_IRQn EXTI0_1_IRQn
#define CH2_Pin GPIO_PIN_1
#define CH2_GPIO_Port GPIOB
#define CH2_EXTI_IRQn EXTI0_1_IRQn
#define CH3_Pin GPIO_PIN_2
#define CH3_GPIO_Port GPIOB
#define CH3_EXTI_IRQn EXTI2_3_IRQn
#define SLEEPN_M3_Pin GPIO_PIN_10
#define SLEEPN_M3_GPIO_Port GPIOB
#define PWM_M3_B_Pin GPIO_PIN_11
#define PWM_M3_B_GPIO_Port GPIOB
#define SLEEPN_M2_Pin GPIO_PIN_12
#define SLEEPN_M2_GPIO_Port GPIOB
#define PWM_M2_B_Pin GPIO_PIN_13
#define PWM_M2_B_GPIO_Port GPIOB
#define SLEEPN_M1_Pin GPIO_PIN_14
#define SLEEPN_M1_GPIO_Port GPIOB
#define PWM_M1_B_Pin GPIO_PIN_15
#define PWM_M1_B_GPIO_Port GPIOB
#define PWM_M1_F_Pin GPIO_PIN_8
#define PWM_M1_F_GPIO_Port GPIOA
#define PWM_M2_F_Pin GPIO_PIN_9
#define PWM_M2_F_GPIO_Port GPIOA
#define PWM_M3_F_Pin GPIO_PIN_10
#define PWM_M3_F_GPIO_Port GPIOA
#define CH4_Pin GPIO_PIN_3
#define CH4_GPIO_Port GPIOB
#define CH4_EXTI_IRQn EXTI2_3_IRQn
#define SERVO1_HV_Pin GPIO_PIN_6
#define SERVO1_HV_GPIO_Port GPIOB
#define SERVO2_Pin GPIO_PIN_7
#define SERVO2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define TRUE 1
#define FALSE 0
// ----------- SETTINGS ----------- //
// RECIEVER
#define NUMBER_OF_PPM_CHANNEL 8
// SERVOS
#define S1_MIN_TIME_PWM	1000
#define S1_MAX_TIME_PWM	2000
#define S2_MIN_TIME_PWM	544
#define S2_MAX_TIME_PWM	2400
// BATTERY
#define CUTOFF_BATTERY_TIMEOUT 10	// seconds at witch the battery must be below the minimum to be declared empty
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
