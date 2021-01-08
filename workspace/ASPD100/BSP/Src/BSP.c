//BSP mapping functions

#include <IRQ.h>
#include "BSP.h"
//#include "Setup.h"
//#include "history.hpp"
//#include "Pins.h"
#include "main.h"
//#include "history.hpp"
//#include "Model_Config.h"
#include "I2C_Wrapper.h"

void resetWatchdog() {
	//HAL_IWDG_Refresh(&hiwdg);
}

void unstick_I2C() {
	GPIO_InitTypeDef GPIO_InitStruct;
	int timeout = 100;
	int timeout_cnt = 0;

// 1. Clear PE bit.
	hi2c1.Instance->CR1 &= ~(0x0001);
	/**I2C1 GPIO Configuration
	 PB6     ------> I2C1_SCL
	 PB7     ------> I2C1_SDA
	 */
//  2. Configure the SCL and SDA I/Os as General Purpose Output Open-Drain, High level (Write 1 to GPIOx_ODR).
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	GPIO_InitStruct.Pin = I2C1_SCL_Pin;
	HAL_GPIO_Init(I2C1_SCL_GPIO_Port, &GPIO_InitStruct);
	HAL_GPIO_WritePin(I2C1_SCL_GPIO_Port, I2C1_SCL_Pin, GPIO_PIN_SET);

	GPIO_InitStruct.Pin = I2C1_SDA_Pin;
	HAL_GPIO_Init(I2C1_SDA_GPIO_Port, &GPIO_InitStruct);
	HAL_GPIO_WritePin(I2C1_SDA_GPIO_Port, I2C1_SDA_Pin, GPIO_PIN_SET);

	while (GPIO_PIN_SET != HAL_GPIO_ReadPin(I2C1_SDA_GPIO_Port, I2C1_SDA_Pin)) {
		//Move clock to release I2C
		HAL_GPIO_WritePin(I2C1_SCL_GPIO_Port, I2C1_SCL_Pin, GPIO_PIN_RESET);
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		HAL_GPIO_WritePin(I2C1_SCL_GPIO_Port, I2C1_SCL_Pin, GPIO_PIN_SET);

		timeout_cnt++;
		if (timeout_cnt > timeout)
			return;
	}

// 12. Configure the SCL and SDA I/Os as Alternate function Open-Drain.
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	GPIO_InitStruct.Pin = I2C1_SCL_Pin;
	HAL_GPIO_Init(I2C1_SCL_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = I2C1_SDA_Pin;
	HAL_GPIO_Init(I2C1_SDA_GPIO_Port, &GPIO_InitStruct);

	HAL_GPIO_WritePin(I2C1_SCL_GPIO_Port, I2C1_SCL_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(I2C1_SDA_GPIO_Port, I2C1_SDA_Pin, GPIO_PIN_SET);

// 13. Set SWRST bit in I2Cx_CR1 register.
	hi2c1.Instance->CR1 |= 0x8000;

	asm("nop");

// 14. Clear SWRST bit in I2Cx_CR1 register.
	hi2c1.Instance->CR1 &= ~0x8000;

	asm("nop");

// 15. Enable the I2C peripheral by setting the PE bit in I2Cx_CR1 register
	hi2c1.Instance->CR1 |= 0x0001;

// Call initialization function.
	HAL_I2C_Init(&hi2c1);
}

uint8_t getButtonRight() {
	return HAL_GPIO_ReadPin(rightButton_GPIO_Port, rightButton_Pin) == GPIO_PIN_RESET ?
			1 : 0;
}

uint8_t getButtonLeft() {
	return HAL_GPIO_ReadPin(leftButton_GPIO_Port, leftButton_Pin) == GPIO_PIN_RESET ?
			1 : 0;
}

uint8_t getButtonUp() {
	return HAL_GPIO_ReadPin(upButton_GPIO_Port, upButton_Pin) == GPIO_PIN_RESET ?
			1 : 0;
}

uint8_t getButtonDown() {
	return HAL_GPIO_ReadPin(downButton_GPIO_Port, downButton_Pin) == GPIO_PIN_RESET ?
			1 : 0;
}

uint8_t getButtonCenter() {
	return HAL_GPIO_ReadPin(centerButton_GPIO_Port, centerButton_Pin) == GPIO_PIN_RESET ?
			1 : 0;
}

void reboot() {

	NVIC_SystemReset();
}

void delay_ms(uint16_t count) {
	HAL_Delay(count);
}
