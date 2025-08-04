/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_Pin GPIO_PIN_14
#define LED_GPIO_Port GPIOC
#define DIGITAL_AUX1_Pin GPIO_PIN_0
#define DIGITAL_AUX1_GPIO_Port GPIOF
#define DIGITAL_AUX2_Pin GPIO_PIN_1
#define DIGITAL_AUX2_GPIO_Port GPIOF
#define VBAT_Pin GPIO_PIN_0
#define VBAT_GPIO_Port GPIOA
#define ADC_AUX_Pin GPIO_PIN_1
#define ADC_AUX_GPIO_Port GPIOA
#define M1_F_Pin GPIO_PIN_2
#define M1_F_GPIO_Port GPIOA
#define M1_B_Pin GPIO_PIN_3
#define M1_B_GPIO_Port GPIOA
#define M2_F_Pin GPIO_PIN_4
#define M2_F_GPIO_Port GPIOA
#define M2_B_Pin GPIO_PIN_5
#define M2_B_GPIO_Port GPIOA
#define M3_F_Pin GPIO_PIN_6
#define M3_F_GPIO_Port GPIOA
#define M3_B_Pin GPIO_PIN_7
#define M3_B_GPIO_Port GPIOA
#define SERVO1_Pin GPIO_PIN_0
#define SERVO1_GPIO_Port GPIOB
#define SERVO2_Pin GPIO_PIN_1
#define SERVO2_GPIO_Port GPIOB
#define NRF24L01_CE_Pin GPIO_PIN_8
#define NRF24L01_CE_GPIO_Port GPIOA
#define CH4_Pin GPIO_PIN_15
#define CH4_GPIO_Port GPIOA
#define CH4_EXTI_IRQn EXTI4_15_IRQn
#define CH3_Pin GPIO_PIN_3
#define CH3_GPIO_Port GPIOB
#define CH3_EXTI_IRQn EXTI2_3_IRQn
#define CH2_Pin GPIO_PIN_4
#define CH2_GPIO_Port GPIOB
#define CH2_EXTI_IRQn EXTI4_15_IRQn
#define CH1_Pin GPIO_PIN_5
#define CH1_GPIO_Port GPIOB
#define CH1_EXTI_IRQn EXTI4_15_IRQn

/* USER CODE BEGIN Private defines */
#define FALSE 0
#define TRUE 1
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
