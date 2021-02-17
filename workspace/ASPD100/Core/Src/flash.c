/*
 * flash.c
 *
 *  Created on: 29 May 2020
 *      Author: Ralim
 */

#include "BSP_Flash.h"
//#include "BSP.h"
#include "string.h"
#include "stm32l4xx_hal.h"

static uint16_t settings_page[256] __attribute__ ((section (".settings_page"), used));

/**
  * @brief  Gets the page of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The page of a given address
  */
static uint32_t GetPage(uint32_t Addr)
{
  uint32_t page = 0;

  if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
  {
    /* Bank 1 */
    page = (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;
  }
  else
  {
    /* Bank 2 */
    page = (Addr - (FLASH_BASE + FLASH_BANK_SIZE)) / FLASH_PAGE_SIZE;
  }

  return page;
}

/**
  * @brief  Gets the bank of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The bank of a given address
  */
static uint32_t GetBank(uint32_t Addr)
{
  return FLASH_BANK_1;
}

uint8_t flash_save_buffer(const uint8_t *buffer, const uint16_t length) {

	uint32_t PAGEError = 0, FirstPage = 0, BankNumber = 0;

	/* Unlock the Flash to enable the flash control register access *************/
	HAL_FLASH_Unlock();

	/* Get the 1st page to erase */
	FirstPage = GetPage((uint32_t)settings_page);

	/* Get the bank */
	BankNumber = GetBank((uint32_t)settings_page);

	FLASH_EraseInitTypeDef EraseInitStruct;

	EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.Banks = BankNumber;
	EraseInitStruct.Page = FirstPage;
	EraseInitStruct.NbPages = 1;

	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_BSY);
	/* Unlock the Flash to enable the flash control register access *************/
	HAL_FLASH_Unlock();
	HAL_Delay(1);

	// Erase the page of flash (2048 bytes on this stm32)
	if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
	{
		/*Error occurred while page erase.*/
		return HAL_FLASH_GetError ();
	}

    /* Unlock the Flash to enable the flash control register access *************/
	HAL_FLASH_Unlock();

    uint64_t *data = (uint64_t*) buffer;

	/*
	 * Length is divide by 8 because we program double word.
	 * Word = 4 byte, Double Word = 8 byte.
	 */
	for (uint8_t i = 0; i < (length / 8); i++) {
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, (uint32_t)&settings_page[i + (3*i)], data[i]);
		FLASH_WaitForLastOperation(1000);
	}

	/* Lock the Flash to disable the flash control register access (recommended
	 * to protect the FLASH memory against possible unwanted operation) ********
	 */
	HAL_FLASH_Lock();

	return 1;
}

void flash_read_buffer(uint8_t *buffer, const uint16_t length) {

	memcpy(buffer, settings_page, length);
}
