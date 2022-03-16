/*
 * FRToSI2C.c
 *
 *  Created on: 24 sept. 2020
 *      Author: Alan
 */
#include "I2C_Wrapper.h"
#include "BSP.h"

static void FRToSI2C_unlock();
static bool FRToSI2C_lock();
static void FRToSI2C_I2C_Unstick();

void FRToSI2C_CpltCallback() {
	hi2c1.State = HAL_I2C_STATE_READY;  // Force state reset (even if tx error)
	if (FRToSI2C_I2CSemaphoreHandle) {
		//xSemaphoreGiveFromISR(I2CSemaphore, NULL);
		osSemaphoreRelease(FRToSI2C_I2CSemaphoreHandle);
	}
}

bool FRToSI2C_Mem_Read(uint16_t DevAddress, uint16_t MemAddress,
		uint8_t *pData, uint16_t Size) {

	if (FRToSI2C_lock() != osOK)
		return false;
	if (HAL_I2C_Mem_Read(&hi2c1, DevAddress, MemAddress,
	I2C_MEMADD_SIZE_8BIT, pData, Size, 500) != HAL_OK) {

		FRToSI2C_I2C_Unstick();
		FRToSI2C_unlock();
		return false;
	}

	FRToSI2C_unlock();
	return true;

}

bool FRToSI2C_I2C_RegisterWrite(uint8_t address, uint8_t reg, uint8_t data) {
	return FRToSI2C_Mem_Write(address, reg, &data, 1);
}

uint8_t FRToSI2C_I2C_RegisterRead(uint8_t add, uint8_t reg) {
	uint8_t tx_data[1];
	FRToSI2C_Mem_Read(add, reg, tx_data, 1);
	return tx_data[0];
}

bool FRToSI2C_Mem_Write(uint16_t DevAddress, uint16_t MemAddress,
		uint8_t *pData, uint16_t Size) {

	if (FRToSI2C_lock() != osOK)
		return false;
	if (HAL_I2C_Mem_Write(&hi2c1, DevAddress, MemAddress,
	I2C_MEMADD_SIZE_8BIT, pData, Size, 500) != HAL_OK) {

		FRToSI2C_I2C_Unstick();
		FRToSI2C_unlock();
		return false;
	}

	FRToSI2C_unlock();
	return true;
}

bool I2C_Transmit(uint16_t DevAddress, uint8_t *pData, uint16_t Size) {
	if (FRToSI2C_lock() != osOK)
		return false;
	if (HAL_I2C_Master_Transmit_DMA(&hi2c1, DevAddress, pData, Size)
			!= HAL_OK) {
		FRToSI2C_I2C_Unstick();
		FRToSI2C_unlock();
		return false;
	}
	return true;
}

bool FRToSI2C_probe(uint16_t DevAddress) {
	if (FRToSI2C_lock() != osOK)
		return false;
	uint8_t buffer[1];
	bool worked = HAL_I2C_Mem_Read(&hi2c1, DevAddress, 0x0F,
			I2C_MEMADD_SIZE_8BIT, buffer, 1, 1000) == HAL_OK;
	FRToSI2C_unlock();
	return worked;
}

void FRToSI2C_I2C_Unstick() {
	unstick_I2C();
}

void FRToSI2C_unlock() {

	osSemaphoreRelease(FRToSI2C_I2CSemaphoreHandle);
}

bool FRToSI2C_lock() {

	return osSemaphoreAcquire(FRToSI2C_I2CSemaphoreHandle, (TickType_t)50) == pdTRUE;
}
