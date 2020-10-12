/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "gui.h"
#include "Buttons.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticSemaphore_t osStaticSemaphoreDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for GUITask */
osThreadId_t GUITaskHandle;
const osThreadAttr_t GUITask_attributes = {
  .name = "GUITask",
  .priority = (osPriority_t) osPriorityBelowNormal,
  .stack_size = 1024 * 4
};
/* Definitions for FRToSI2C_I2CSemaphore */
osSemaphoreId_t FRToSI2C_I2CSemaphoreHandle;
osStaticSemaphoreDef_t FRToSI2C_xSemaphoreBuffer;
const osSemaphoreAttr_t FRToSI2C_I2CSemaphore_attributes = {
  .name = "FRToSI2C_I2CSemaphore",
  .cb_mem = &FRToSI2C_xSemaphoreBuffer,
  .cb_size = sizeof(FRToSI2C_xSemaphoreBuffer),
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartGUITask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of FRToSI2C_I2CSemaphore */
  FRToSI2C_I2CSemaphoreHandle = osSemaphoreNew(1, 1, &FRToSI2C_I2CSemaphore_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of GUITask */
  GUITaskHandle = osThreadNew(StartGUITask, NULL, &GUITask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

void GUIDelay() {
	// Called in all UI looping tasks,
	// This limits the re-draw rate to the LCD and also lets the DMA run
	// As the gui task can very easily fill this bus with transactions, which will
	// prevent the movement detection from running
	osDelay(50);
}

/* USER CODE BEGIN Header_StartGUITask */
/**
  * @brief  Function implementing the GUITask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartGUITask */
void StartGUITask(void *argument)
{
  /* USER CODE BEGIN StartGUITask */
  /* Infinite loop */
	OLED_initialize();  // start up the OLED screen

	bool buttonLockout = false;

	for (;;) {

#if 1
		enterRootMenu();  // enter the settings menu
		buttonLockout = true;
#elif 1
		OLED_fullScreen();
		OLED_refresh();
		osDelay(1000);
		OLED_clearScreen();
		OLED_refresh();
		osDelay(1000);
#else
		ButtonState buttons = getButtonState();
		if (buttons != BUTTON_NONE) {
			OLED_setDisplayState(ON);
			OLED_setFont(0);
		}

		if (buttons != BUTTON_NONE && buttonLockout)
			buttons = BUTTON_NONE;
		else
			buttonLockout = false;

		switch (buttons) {
			case BUTTON_NONE:
				// Do nothing
				break;
			case BUTTON_BOTH:
				// Not used yet
				// In multi-language this might be used to reset language on a long hold
				// or some such
				printf("BUTTON_BOTH\r\n");
				break;

			case BUTTON_L_LONG:
				printf("BUTTON_L_LONG\r\n");

				break;
			case BUTTON_R_LONG:
				printf("BUTTON_R_LONG\r\n");

				break;
			case BUTTON_L_SHORT:
				printf("BUTTON_L_SHORT\r\n");

				break;
			case BUTTON_R_SHORT:
				printf("BUTTON_R_SHORT\r\n");
				enterRootMenu();  // enter the settings menu
				buttonLockout = true;
				break;
			default:
				break;
		}
#endif

		OLED_refresh();
		GUIDelay();
	}
  /* USER CODE END StartGUITask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
