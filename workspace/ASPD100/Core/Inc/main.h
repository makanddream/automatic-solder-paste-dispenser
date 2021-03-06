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
#include "usart.h"
#include "gpio.h"
#include "adc.h"

#include "OLED.h"
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
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void startGUITask(void *argument);
void StartMotorTask(void *argument);
extern TaskHandle_t motorTaskNotification;
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define counterPeriod 100
#define counterPeriod2 50
#define MCO_Pin GPIO_PIN_0
#define MCO_GPIO_Port GPIOA
#define IPROPI_Pin GPIO_PIN_1
#define IPROPI_GPIO_Port GPIOA
#define VCP_TX_Pin GPIO_PIN_2
#define VCP_TX_GPIO_Port GPIOA
#define upButton_Pin GPIO_PIN_3
#define upButton_GPIO_Port GPIOA
#define rightButton_Pin GPIO_PIN_4
#define rightButton_GPIO_Port GPIOA
#define centerButton_Pin GPIO_PIN_5
#define centerButton_GPIO_Port GPIOA
#define leftButton_Pin GPIO_PIN_6
#define leftButton_GPIO_Port GPIOA
#define downButton_Pin GPIO_PIN_7
#define downButton_GPIO_Port GPIOA
#define actionButton_Pin GPIO_PIN_0
#define actionButton_GPIO_Port GPIOB
#define actionButton_EXTI_IRQn EXTI0_IRQn
#define PH_IN2_Pin GPIO_PIN_1
#define PH_IN2_GPIO_Port GPIOB
#define EN_IN1_Pin GPIO_PIN_8
#define EN_IN1_GPIO_Port GPIOA
#define I2C1_SCL_Pin GPIO_PIN_9
#define I2C1_SCL_GPIO_Port GPIOA
#define I2C1_SDA_Pin GPIO_PIN_10
#define I2C1_SDA_GPIO_Port GPIOA
#define nSLEEP_Pin GPIO_PIN_11
#define nSLEEP_GPIO_Port GPIOA
#define nFAULT_Pin GPIO_PIN_12
#define nFAULT_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define VCP_RX_Pin GPIO_PIN_15
#define VCP_RX_GPIO_Port GPIOA
#define LD3_Pin GPIO_PIN_3
#define LD3_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
