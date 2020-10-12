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
}
