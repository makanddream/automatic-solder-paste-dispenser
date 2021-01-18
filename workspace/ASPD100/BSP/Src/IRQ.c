/*
 * IRQ.c
 *
 *  Created on: 24 sept. 2020
 *      Author: Alan
 */

#include "IRQ.h"
/*
 * Catch the IRQ
 */
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c) {
	FRToSI2C_CpltCallback();
}
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c) {
	FRToSI2C_CpltCallback();
}
void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c) {
	FRToSI2C_CpltCallback();
}
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c) {
	FRToSI2C_CpltCallback();
}
void HAL_I2C_AbortCpltCallback(I2C_HandleTypeDef *hi2c) {
	FRToSI2C_CpltCallback();
}
void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c) {
	FRToSI2C_CpltCallback();
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	(void)GPIO_Pin;
	//InterruptHandler_irqCallback();
	if(GPIO_Pin == actionButton_Pin){
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		if (motorTaskNotification) {
			vTaskNotifyGiveFromISR(motorTaskNotification,
						&xHigherPriorityTaskWoken);
			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
		}
	}
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM7 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }else if (htim->Instance == TIM7) {
	  if(!getButtonAction()){
		  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
		  HAL_TIM_Base_Stop_IT(&htim7); //TODO need upgrade to avoid the bug timer
	  }
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}
