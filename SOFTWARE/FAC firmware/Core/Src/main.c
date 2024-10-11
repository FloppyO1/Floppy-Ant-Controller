/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc;

I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim16;
TIM_HandleTypeDef htim17;

/* USER CODE BEGIN PV */
const uint8_t CODE_VERSION = 1;
const uint8_t CODE_SUBVERSION = 0;

// main code global variables
TankMotors tank;
uint8_t commandRecieved = 0;
uint16_t batteryVoltage = 0;
uint8_t cutoff = FALSE;
uint8_t limit = FALSE;
uint8_t batteryConfiguration = 0;	// 1=1S, 2=2S,0 not battery poweredx
uint8_t armed = FALSE;

// settings variables
uint8_t thChannel = 2;	// defines the channel of the throttle
uint8_t stChannel = 4;
uint8_t wpChannel = 3;
uint8_t s1Channel = 3;
uint8_t s2Channel = 3;
uint8_t armChannel = 5;

uint8_t m1Rev = 1;	// defines if the motor 1 needs to be reversed
uint8_t m2Rev = 1;
uint8_t m3Rev = 0;
uint8_t s1Rev = 0;
uint8_t s2Rev = 0;

uint8_t mLeft = M1;	// defines that the motor 1 is the left motor
uint8_t mRight = M2;
uint8_t mWeapon = M3;

uint8_t weaponDoubleDirection = 0;	// defines if the weapon is double direction

uint8_t tankMixIsON = 1;	// defines if the tank mix is on(1) or off(0)

uint8_t noDisarm = 1;	// if 1 is always armed

uint8_t limitVoltage = 37;	// 3200mV (3.3V)
uint8_t cutOffVoltage = 34;	// 2800mV (2.8V)

uint_fast8_t recieverIsPWM = 1;	// if 1 the reciever must output four channel in PWM mode, else PPM on ch1
// for debug and tests
//  |
//  V
uint16_t ccr1;
uint16_t ccr2;
uint16_t ccr3;
uint16_t ccrservo1;
uint16_t ccrservo2;

uint8_t provaDatoEeprom;

uint8_t recievedString[20];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM16_Init(void);
static void MX_TIM17_Init(void);
static void MX_TIM1_Init(void);
static void MX_I2C1_Init(void);
static void MX_ADC_Init(void);
static void MX_TIM6_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {

	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_TIM16_Init();
	MX_TIM17_Init();
	MX_TIM1_Init();
	MX_I2C1_Init();
	MX_USB_DEVICE_Init();
	MX_ADC_Init();
	MX_TIM6_Init();
	/* USER CODE BEGIN 2 */

	/* ------------- ROBOT CONTROL CODE --------------- */
	// HAL_Delay(3000);
	initReciever(&htim6);
	initMotors(&htim1);
	initBattery(&hadc);
	batteryConfiguration = getBatteryConfiguration();

	// load settings
	//write_byte_eeprom(EEPROM_ISFIRSTBOOT_ADDRESS, 255);
	/* !!!!!!!!!!!!!!!!!! RICORDA DI SCOMMENTARE !!!!!!!!!!!!!!!!!!!!!!!!!*///               <------------------------
	uint8_t isFirstWrite = read_byte_eeprom(EEPROM_ISFIRSTBOOT_ADDRESS);
	if (isFirstWrite) {	// if is the first time the value is 255 (value of the memory empty)
		for (int i = 0; i < 10; i++) {
			HAL_GPIO_TogglePin(U_LED_GPIO_Port, U_LED_Pin);
			HAL_Delay(100);
		}
		storeSettingsToEeprom();	// write the default settings
		isFirstWrite = 0;	// next boot it will load the settings from eeprom memory
		write_byte_eeprom(EEPROM_ISFIRSTBOOT_ADDRESS, isFirstWrite);
	} else {
		loadSettingsFromEeprom();	// load the settings from the eeprom memory
	}
	// disable all the dc motors
	disableMotor(M1);
	disableMotor(M2);
	disableMotor(M3);

	uint32_t time1 = 0;
	uint32_t time2 = 0;
	uint32_t time3 = 0;
	uint32_t timeLimitBattery = 0;
	uint32_t timeLowBattery = 0;

	/* ------------- TEST/DEBUG CODE --------------- */

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* ------------- ROBOT CONTROL CODE --------------- */

		if (!cutoff) {	// if the robot is not in cutoff
			// arm/disarm robot
			if (!noDisarm) {
				if (getChannelValuePercentage(armChannel) > 75)
					armed = TRUE;
				else
					armed = FALSE;
			}

			if (!armed) {	// if the robot is not already armed
				makeSound(mLeft, 50);
				HAL_Delay(50);
				if (noDisarm) {	// if the throttle steering channels are at center, arm the robot
					if (checkChannelOnCenter(thChannel) && checkChannelOnCenter(stChannel)) armed = TRUE;
				}
			}

			if (armed) {	// if armed do...
				// enable DC motors
				enableMotor(M1);
				enableMotor(M2);
				enableMotor(M3);
				// enable servos
				setServoIsEnable(TRUE, &htim17); // start servo 1 HV
				setServoIsEnable(TRUE, &htim16); // start servo 2

				// tank mix and movements
				if (tankMixIsON) {	// calculate and use the tank mix only if it's enabled
					tank = tankMix(getChannelValuePercentage(stChannel), getChannelValuePercentage(thChannel));
					setTankMotor(tank);
				} else {	// if tank off, th->m1, st->m2, wp->m3
					setMotorSpeedBidirectional(1, getChannelValuePercentage(thChannel));
					setMotorSpeedBidirectional(2, getChannelValuePercentage(stChannel));
					setMotorSpeedBidirectional(3, getChannelValuePercentage(wpChannel));
				}

				// weapons
				if (weaponDoubleDirection) {
					setMotorSpeedBidirectional(mWeapon, getChannelValuePercentage(wpChannel));
				} else {
					uint8_t wp = getChannelValuePercentage(wpChannel);
					if (limit == TRUE) wp = wp / 2;	// valid only in the unidirectional mode  !!FOR DC MOTOR ONLY!!
					setMotorSpeedUnidirectional(mWeapon, wp);
				}

				// set the position of servos
				if (limit == TRUE) { // !!ASUME THAT S1 IS CONNECTED TO THE WEAPON ESC
					uint8_t s1 = getChannelValuePercentage(s1Channel);
					if (limit == TRUE) s1 = s1 / 2;
					setServoAngle(S1, s1); // servo 1 attached to channel s1Channel
				} else {
					setServoAngle(S1, getChannelValuePercentage(s1Channel));	// servo 1 attached to channel s1Channel
				}
				setServoAngle(S2, getChannelValuePercentage(s2Channel));
			} else {	// if disarmed do...
				//disable DC motors
				disableMotor(M1);
				disableMotor(M2);
				disableMotor(M3);
				// turn of the servos signals
				setServoIsEnable(FALSE, &htim17); // stop servo 1 HV
				setServoIsEnable(FALSE, &htim16); // stop servo 2
			} // end if disarmed

			// battery control every 200ms for cutoff & limit
			if (HAL_GetTick() - time2 >= 200) {
				batteryVoltage = getBattVoltage();
				// send battery voltage
//				uint8_t s[20];
//				sprintf(s, "%dS Vbatt = %d mV\n", batteryConfiguration, batteryVoltage);
//				serialPrintString(s);
				// limit weapon speed if low battery
				if (HAL_GetTick() - timeLimitBattery >= CUTOFF_BATTERY_TIMEOUT * 1000) {
					limit = TRUE;	// activate limit state (weapon speed limited)
				}
				if (batteryVoltage >= limitVoltage * 100 * batteryConfiguration && !limit)
					timeLimitBattery = HAL_GetTick() + CUTOFF_BATTERY_TIMEOUT;

				// battery voltage & low voltage detection
				if (HAL_GetTick() - timeLowBattery >= CUTOFF_BATTERY_TIMEOUT * 1000) {
					cutoff = TRUE;	// activate cutoff state (robot disarmed)
				}
				// if the battery voltage is over the cutoff voltage, reset the timer
				if (batteryVoltage >= cutOffVoltage * 100 * batteryConfiguration && !cutoff)
					timeLowBattery = HAL_GetTick() + CUTOFF_BATTERY_TIMEOUT;
				// reset the time
				time2 = HAL_GetTick();
			}

			// led blinking with a period of 4s (normal operation)
			if (HAL_GetTick() - time1 >= 2000 && limit == FALSE) {
				// led indicator
				HAL_GPIO_TogglePin(U_LED_GPIO_Port, U_LED_Pin);
				// reset the time
				time1 = HAL_GetTick();
			}
			// led blinking with a period of 750ms (limit active)
			if (HAL_GetTick() - time3 >= 350 && limit == TRUE) {
				// led indicator
				HAL_GPIO_TogglePin(U_LED_GPIO_Port, U_LED_Pin);
				// reset the time
				time3 = HAL_GetTick();
			}
		} else {	// if robot in cutoff mode
			//disable DC motors
			disableMotor(M1);
			disableMotor(M2);
			disableMotor(M3);
			// turn of the servos signals
			setServoAngle(S1, 0);	// servo 1 attached to channel s1Channel
			setServoAngle(S2, 0);
			if (HAL_GetTick() - time1 >= 100) {
				// led indicator
				HAL_GPIO_TogglePin(U_LED_GPIO_Port, U_LED_Pin);
				makeSound(mLeft, 100);	// beep while in cutoff mode
				// reset the time
				time1 = HAL_GetTick();
			}
		}

		// Can always accept command from the USB port
		if (commandRecieved) {	// recieved a command from the USB
			if (modifySettingsWithCommand()) {
				uint8_t s[] = "\nOK\n";
				serialPrintString(s);
				//serialPrintSettings();	// not needed
			} else {
				uint8_t s[] = "\nWHAT??\n";
				serialPrintString(s);
			}
			commandRecieved = FALSE;
		}

		/* ------------- TEST/DEBUG CODE --------------- */
//		enableMotor(M1);
//		enableMotor(M2);
//		enableMotor(M3);
//
//		for (int i = 0; i < 100; i++) {
//			setMotorSpeedUnidirectional(M1, i);
//			setMotorSpeedUnidirectional(M2, i);
//			setMotorSpeedUnidirectional(M3, i);
//			setServoAngle(1, i);
//			setServoAngle(2, i);
//			HAL_Delay(10);
//		}
//
//		ccr1 = TIM1->CCR1;
//		ccr2 = TIM1->CCR2;
//		ccr3 = TIM1->CCR3;
//		ccrservo1 = TIM16->CCR1;
//		ccrservo2 = TIM17->CCR1;
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
	RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSI14 | RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.HSI14CalibrationValue = 16;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
	RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK) {
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB | RCC_PERIPHCLK_I2C1;
	PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
	PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL;

	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief ADC Initialization Function
 * @param None
 * @retval None
 */
static void MX_ADC_Init(void) {

	/* USER CODE BEGIN ADC_Init 0 */

	/* USER CODE END ADC_Init 0 */

	ADC_ChannelConfTypeDef sConfig = { 0 };

	/* USER CODE BEGIN ADC_Init 1 */

	/* USER CODE END ADC_Init 1 */

	/** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
	 */
	hadc.Instance = ADC1;
	hadc.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
	hadc.Init.Resolution = ADC_RESOLUTION_12B;
	hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
	hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hadc.Init.LowPowerAutoWait = DISABLE;
	hadc.Init.LowPowerAutoPowerOff = DISABLE;
	hadc.Init.ContinuousConvMode = DISABLE;
	hadc.Init.DiscontinuousConvMode = ENABLE;
	hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc.Init.DMAContinuousRequests = DISABLE;
	hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
	if (HAL_ADC_Init(&hadc) != HAL_OK) {
		Error_Handler();
	}

	/** Configure for the selected ADC regular channel to be converted.
	 */
	sConfig.Channel = ADC_CHANNEL_0;
	sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
	sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
	if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN ADC_Init 2 */

	/* USER CODE END ADC_Init 2 */

}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void) {

	/* USER CODE BEGIN I2C1_Init 0 */

	/* USER CODE END I2C1_Init 0 */

	/* USER CODE BEGIN I2C1_Init 1 */

	/* USER CODE END I2C1_Init 1 */
	hi2c1.Instance = I2C1;
	hi2c1.Init.Timing = 0x00201D2B;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
		Error_Handler();
	}

	/** Configure Analogue filter
	 */
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK) {
		Error_Handler();
	}

	/** Configure Digital filter
	 */
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN I2C1_Init 2 */

	/* USER CODE END I2C1_Init 2 */

}

/**
 * @brief TIM1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM1_Init(void) {

	/* USER CODE BEGIN TIM1_Init 0 */

	/* USER CODE END TIM1_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };
	TIM_OC_InitTypeDef sConfigOC = { 0 };
	TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = { 0 };

	/* USER CODE BEGIN TIM1_Init 1 */

	/* USER CODE END TIM1_Init 1 */
	htim1.Instance = TIM1;
	htim1.Init.Prescaler = 480 - 1;
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim1.Init.Period = 100 - 1;
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1.Init.RepetitionCounter = 0;
	htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim1) != HAL_OK) {
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_TIM_PWM_Init(&htim1) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK) {
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3) != HAL_OK) {
		Error_Handler();
	}
	sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
	sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
	sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
	sBreakDeadTimeConfig.DeadTime = 0;
	sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
	sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
	sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
	if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM1_Init 2 */

	/* USER CODE END TIM1_Init 2 */
	HAL_TIM_MspPostInit(&htim1);

}

/**
 * @brief TIM6 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM6_Init(void) {

	/* USER CODE BEGIN TIM6_Init 0 */

	/* USER CODE END TIM6_Init 0 */

	/* USER CODE BEGIN TIM6_Init 1 */

	/* USER CODE END TIM6_Init 1 */
	htim6.Instance = TIM6;
	htim6.Init.Prescaler = 48 - 1;
	htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim6.Init.Period = 65535;
	htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim6) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM6_Init 2 */

	/* USER CODE END TIM6_Init 2 */

}

/**
 * @brief TIM16 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM16_Init(void) {

	/* USER CODE BEGIN TIM16_Init 0 */

	/* USER CODE END TIM16_Init 0 */

	TIM_OC_InitTypeDef sConfigOC = { 0 };
	TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = { 0 };

	/* USER CODE BEGIN TIM16_Init 1 */

	/* USER CODE END TIM16_Init 1 */
	htim16.Instance = TIM16;
	htim16.Init.Prescaler = 24 - 1;
	htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim16.Init.Period = 10000 - 1;
	htim16.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim16.Init.RepetitionCounter = 0;
	htim16.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim16) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_TIM_PWM_Init(&htim16) != HAL_OK) {
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	if (HAL_TIM_PWM_ConfigChannel(&htim16, &sConfigOC, TIM_CHANNEL_1) != HAL_OK) {
		Error_Handler();
	}
	sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
	sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
	sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
	sBreakDeadTimeConfig.DeadTime = 0;
	sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
	sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
	sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
	if (HAL_TIMEx_ConfigBreakDeadTime(&htim16, &sBreakDeadTimeConfig) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM16_Init 2 */

	/* USER CODE END TIM16_Init 2 */
	HAL_TIM_MspPostInit(&htim16);

}

/**
 * @brief TIM17 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM17_Init(void) {

	/* USER CODE BEGIN TIM17_Init 0 */

	/* USER CODE END TIM17_Init 0 */

	TIM_OC_InitTypeDef sConfigOC = { 0 };
	TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = { 0 };

	/* USER CODE BEGIN TIM17_Init 1 */

	/* USER CODE END TIM17_Init 1 */
	htim17.Instance = TIM17;
	htim17.Init.Prescaler = 24 - 1;
	htim17.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim17.Init.Period = 10000 - 1;
	htim17.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim17.Init.RepetitionCounter = 0;
	htim17.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim17) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_TIM_PWM_Init(&htim17) != HAL_OK) {
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	if (HAL_TIM_PWM_ConfigChannel(&htim17, &sConfigOC, TIM_CHANNEL_1) != HAL_OK) {
		Error_Handler();
	}
	sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
	sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
	sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
	sBreakDeadTimeConfig.DeadTime = 0;
	sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
	sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
	sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
	if (HAL_TIMEx_ConfigBreakDeadTime(&htim17, &sBreakDeadTimeConfig) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM17_Init 2 */

	/* USER CODE END TIM17_Init 2 */
	HAL_TIM_MspPostInit(&htim17);

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	/* USER CODE BEGIN MX_GPIO_Init_1 */
	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(U_LED_GPIO_Port, U_LED_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, SLEEPN_M3_Pin | DIR_M3_Pin | SLEEPN_M2_Pin | DIR_M2_Pin | SLEEPN_M1_Pin | DIR_M1_Pin,
			GPIO_PIN_RESET);

	/*Configure GPIO pin : U_LED_Pin */
	GPIO_InitStruct.Pin = U_LED_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(U_LED_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : CH1_Pin */
	GPIO_InitStruct.Pin = CH1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(CH1_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : CH2_Pin CH3_Pin CH4_Pin */
	GPIO_InitStruct.Pin = CH2_Pin | CH3_Pin | CH4_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pins : SLEEPN_M3_Pin DIR_M3_Pin SLEEPN_M2_Pin DIR_M2_Pin
	 SLEEPN_M1_Pin DIR_M1_Pin */
	GPIO_InitStruct.Pin = SLEEPN_M3_Pin | DIR_M3_Pin | SLEEPN_M2_Pin | DIR_M2_Pin | SLEEPN_M1_Pin | DIR_M1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI0_1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);

	HAL_NVIC_SetPriority(EXTI2_3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI2_3_IRQn);

	/* USER CODE BEGIN MX_GPIO_Init_2 */
	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
int16_t map(int16_t x, int16_t in_min, int16_t in_max, int16_t out_min, int16_t out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

uint8_t checkChannelOnCenter(uint8_t channel) {
	uint8_t position = getChannelValuePercentage(channel);
	if (position >= 50 - 1 && position <= 50 + 1) {	// 50+1 and 50-1 to give some dead-zone
		return TRUE;
	} else {
		return FALSE;
	}
}

uint8_t checkChannelAtZero(uint8_t channel) {
	uint8_t position = getChannelValuePercentage(channel);
	if (position <= 0 + 2) {	// 0+2 to give some dead-zone
		return TRUE;
	} else {
		return FALSE;
	}
}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
		int i = 0;
		i++;
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
