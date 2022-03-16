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
void StartStepperMotorTask(void *argument)
{
	/* USER CODE BEGIN StartMotorTask */
	const TickType_t xBlockTime = pdMS_TO_TICKS( 500 );
	motorTaskNotification = xTaskGetCurrentTaskHandle();

	/* Infinite loop */
	for(;;)
	{
		if (ulTaskNotifyTake(pdTRUE, xBlockTime)) {
			tmc2208_change_direction(&tmc2208, E_FORWARD);

			uint32_t pwmDutyCycle = stepCounterPeriod / 2; // 50%
			// Number of steps performed by the stepper motor
			if (HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, &pwmDutyCycle, aspd100.footPrints[aspd100.indexFootPrint].forwardSteps) != HAL_OK)
			{
				/* PWM Generation Error */
				Error_Handler();
			}
		}
	}
	/* USER CODE END StartMotorTask */
}
