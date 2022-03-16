/*
 * FRToSI2C.h
 *
 *  Created on: 24 sept. 2020
 *      Author: Alan
 */

#ifndef FRTOSI2C_H_
#define FRTOSI2C_H_

#include "cmsis_os.h"
#include "main.h"
/*
 * Wrapper class to work with the device I2C bus
 *
 * This provides mutex protection of the peripheral
 * Also allows hardware to use DMA should it want to
 *
 *
 */

void FRToSI2C_CpltCallback(); //Normal Tx Callback

bool FRToSI2C_Mem_Read(uint16_t DevAddress, uint16_t MemAddress, uint8_t *pData, uint16_t Size);
bool FRToSI2C_Mem_Write(uint16_t DevAddress, uint16_t MemAddress, uint8_t *pData, uint16_t Size);

//Returns true if device ACK's being addressed
bool FRToSI2C_probe(uint16_t DevAddress);

bool I2C_Transmit(uint16_t DevAddress, uint8_t *pData, uint16_t Size);
void FRToSI2C_Receive(uint16_t DevAddress, uint8_t *pData, uint16_t Size);
void I2C_TransmitReceive(uint16_t DevAddress, uint8_t *pData_tx, uint16_t Size_tx, uint8_t *pData_rx, uint16_t Size_rx);
bool FRToSI2C_I2C_RegisterWrite(uint8_t address, uint8_t reg, uint8_t data);
uint8_t FRToSI2C_I2C_RegisterRead(uint8_t address, uint8_t reg);

#endif /* FRTOSI2C_H_ */
