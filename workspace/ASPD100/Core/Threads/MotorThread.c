/*
 * MotorThread.c
 *
 *  Created on: 9 janv. 2021
 *      Author: Alan
 */

#include "main.h"

#include "gui.h"
#include "Buttons.h"

TaskHandle_t motorTaskNotification = NULL;

/* USER CODE BEGIN Header_StartMotorTask */
/**
* @brief Function implementing the MotorTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartMotorTask */
void StartMotorTask(void *argument)
{
	/* USER CODE BEGIN StartMotorTask */
	const TickType_t xBlockTime = pdMS_TO_TICKS( 500 );
	motorTaskNotification = xTaskGetCurrentTaskHandle();

	/* Infinite loop */
	for(;;)
	{
		if (ulTaskNotifyTake(pdTRUE, xBlockTime)) {
			HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
			HAL_TIM_Base_Start_IT(&htim7);
			__HAL_TIM_CLEAR_FLAG(&htim7, TIM_SR_UIF);
		}
	}
	/* USER CODE END StartMotorTask */
}
