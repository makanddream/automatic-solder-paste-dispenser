/*
 * Irqs.h
 *
 *  Created on: 24 sept. 2020
 *      Author: Alan
 */

#ifndef BSP_MINIWARE_IRQ_H_
#define BSP_MINIWARE_IRQ_H_

#include "stm32l4xx_hal.h"
#include "I2C_Wrapper.h"

#ifdef __cplusplus
extern "C" {
#endif
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_AbortCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_GPIO_EXTI_Callback(uint16_t);

#ifdef __cplusplus
}
#endif
#endif /* BSP_MINIWARE_IRQ_H_ */
