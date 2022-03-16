/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>

#include "cmsis_os.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "gpio.h"

#include "OLED.h"
#include "ASPD100.h"
#include "TMC2208.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
extern uint8_t PCBVersion;
extern osSemaphoreId_t FRToSI2C_I2CSemaphoreHandle;
extern StaticSemaphore_t FRToSI2C_xSemaphoreBuffer;

extern aspd100_st aspd100;
extern tmc2208_st tmc2208;
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void StartGUITask(void *argument);
void StartStepperMotorTask(void *argument);
extern TaskHandle_t motorTaskNotification;
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define stepCounterPeriod 200
#define systemCoreClock 64
#define DIR_Pin GPIO_PIN_0
#define DIR_GPIO_Port GPIOA
#define DOWN_BTN_Pin GPIO_PIN_4
#define DOWN_BTN_GPIO_Port GPIOA
#define LEFT_BTN_Pin GPIO_PIN_5
#define LEFT_BTN_GPIO_Port GPIOA
#define CENTER_BTN_Pin GPIO_PIN_6
#define CENTER_BTN_GPIO_Port GPIOA
#define RIGHT_BTN_Pin GPIO_PIN_7
#define RIGHT_BTN_GPIO_Port GPIOA
#define UP_BTN_Pin GPIO_PIN_0
#define UP_BTN_GPIO_Port GPIOB
#define OLED_RST_Pin GPIO_PIN_1
#define OLED_RST_GPIO_Port GPIOB
#define STEP_Pin GPIO_PIN_8
#define STEP_GPIO_Port GPIOA
#define I2C1_SCL_Pin GPIO_PIN_9
#define I2C1_SCL_GPIO_Port GPIOA
#define I2C1_SDA_Pin GPIO_PIN_10
#define I2C1_SDA_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define LEDs_CTR_Pin GPIO_PIN_6
#define LEDs_CTR_GPIO_Port GPIOB
#define ACTION_BTN_Pin GPIO_PIN_7
#define ACTION_BTN_GPIO_Port GPIOB
#define ACTION_BTN_EXTI_IRQn EXTI9_5_IRQn
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
