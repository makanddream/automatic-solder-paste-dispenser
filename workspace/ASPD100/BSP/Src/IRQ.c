/*
 * IRQ.c
 *
 *  Created on: 24 sept. 2020
 *      Author: Alan
 */

#include "IRQ.h"
#include "gui.h"
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

static volatile uint8_t pressed = 0;
static volatile uint8_t released = 0;
static volatile uint8_t click = 0;

static volatile bool isTimerBtnStart = false;
static volatile bool isTimerDebouncingStart = false;
static volatile bool isButtonHold = false;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	(void)GPIO_Pin;
	//InterruptHandler_irqCallback();
	if(GPIO_Pin == actionButton_Pin && !isTimerDebouncingStart){
		__HAL_TIM_SET_COUNTER(&htim15, 0);
		HAL_TIM_Base_Start_IT(&htim15);
		__HAL_TIM_CLEAR_FLAG(&htim15, TIM_SR_UIF);
		isTimerDebouncingStart = true;
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
	  //Timer to turn the motor during x time
	  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
	  HAL_TIM_Base_Stop_IT(&htim7);
  }else if (htim->Instance == TIM15) {
	  //Timer to debounce the action button
	  HAL_TIM_Base_Stop_IT(&htim15);
	  if(isButtonHold){
		  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
		  isButtonHold = false;
	  }else{
		  if(getButtonAction()){
			  //Pressed
			  pressed++;
			  if(!isTimerBtnStart){
				  HAL_TIM_Base_Start_IT(&htim16);
				  __HAL_TIM_SET_COUNTER(&htim16, 0);
				  __HAL_TIM_CLEAR_FLAG(&htim16, TIM_SR_UIF);
				  isTimerBtnStart = true;
				}
		  }else{
			  //Released
			  released++;
		  }

		  if(released > pressed){
			  //Problem detected
			  pressed = released = 0;
		  }else{
			  //Normal operation
			  if(pressed == released && click < 2){
				  click++;
				  pressed = released = 0;
			  }
		  }
	  }
	  isTimerDebouncingStart = false;
  }else if (htim->Instance == TIM16) {
	  HAL_TIM_Base_Stop_IT(&htim16);

	  if(click == 1){
		  //Single click detected
		  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		  if (motorTaskNotification) {
			  vTaskNotifyGiveFromISR(motorTaskNotification,
						&xHigherPriorityTaskWoken);
			  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
		  }
	  }else if(click == 2){
		  //Double click detected
		  counterFootprint++;
	  }else if(pressed == 1){
		  //Hold click detected
		  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
		  isButtonHold = true;
	  }
	  pressed = released = click = 0;
	  isTimerBtnStart = false;
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}
