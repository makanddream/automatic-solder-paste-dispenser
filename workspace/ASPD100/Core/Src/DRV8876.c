/*
 * DRV8876.c
 *
 *  Created on: Jan 6, 2021
 *      Author: Alan
 */

#include "main.h"

#include "DRV8876.h"

void drv8876_driver_start(void){
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_GPIO_WritePin(nSLEEP_GPIO_Port, nSLEEP_Pin, GPIO_PIN_SET);
}

void drv8876_driver_stop(void){
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
}

void drv8876_driver_wakeup(void){
	HAL_GPIO_WritePin(nSLEEP_GPIO_Port, nSLEEP_Pin, GPIO_PIN_SET);
}

void drv8876_driver_sleep(void){
	HAL_GPIO_WritePin(nSLEEP_GPIO_Port, nSLEEP_Pin, GPIO_PIN_RESET);
}

void drv8876_driver_read_current(DRV8876 *drv8876){
	drv8876->currentIPROPI = 12;
}

/*
 * Speed between 0% - 100%
 */

void drv8876_driver_control(uint8_t speed, bool isClockwise){
	if(isClockwise){
		HAL_GPIO_WritePin(PH_IN2_GPIO_Port, PH_IN2_Pin, GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(PH_IN2_GPIO_Port, PH_IN2_Pin, GPIO_PIN_RESET);
	}

	htim1.Instance->CCR1 = (speed * counterPeriod2) / 100;
}

/*
 * Speed between 0% - 100%
 */

void drv8876_speed_control(uint8_t speed){
	htim1.Instance->CCR1 = (speed * counterPeriod2) / 100;
}

/*
 * Speed between 0% - 100%
 */

void drv8876_direction_control(bool isClockwise){

	//True = UP
	//False = DOWN

	if(isClockwise){
		HAL_GPIO_WritePin(PH_IN2_GPIO_Port, PH_IN2_Pin, GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(PH_IN2_GPIO_Port, PH_IN2_Pin, GPIO_PIN_RESET);
	}
}
