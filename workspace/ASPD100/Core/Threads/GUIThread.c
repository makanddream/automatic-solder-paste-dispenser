/*
 * GUIThread.c
 *
 *  Created on: 9 janv. 2021
 *      Author: Alan
 */
#include "main.h"

#include "gui.h"
#include "Buttons.h"

#include "configuration.h"

#include "version.h"

void GUIDelay() {
	// Called in all UI looping tasks,
	// This limits the re-draw rate to the LCD and also lets the DMA run
	// As the gui task can very easily fill this bus with transactions, which will
	// prevent the movement detection from running
	osDelay(50);
}

typedef enum ArrowState_t {
	ARROW_UP, /* TODO */
	ARROW_DOWN, /* TODO */
	ARROW_EMPTY, /* TODO */
}ArrowState;

ButtonState buttonsSave;

static void displayArrow(int16_t x){
	if(buttonsSave == BUTTON_UP_SHORT){
		OLED_drawArea(x, 0, 12, 16, (&arrow[(12 * 2) * 2])); // Arrow UP full
		OLED_drawArea(x, 24, 12, 16, (&arrow[(12 * 2) * 1])); // Arrow DOWN empty
	}else if(buttonsSave == BUTTON_DOWN_SHORT){
		OLED_drawArea(x, 0, 12, 16, (&arrow[(12 * 2) * 0])); // Arrow UP empty
		OLED_drawArea(x, 24, 12, 16, (&arrow[(12 * 2) * 3])); // Arrow DOWN full
	}else if(buttonsSave == BUTTON_NONE){
		OLED_drawArea(x, 0, 12, 16, (&arrow[(12 * 2) * 0])); // Arrow UP empty
		OLED_drawArea(x, 24, 12, 16, (&arrow[(12 * 2) * 1])); // Arrow DOWN empty
	}else{
		// Nothing
	}
	osDelay(50);
}

static void displayArrowState(ArrowState arrowState, int16_t x){
	if(arrowState == ARROW_UP){
		OLED_drawArea(x, 0, 12, 16, (&arrow[(12 * 2) * 2])); // Arrow UP full
		OLED_drawArea(x, 24, 12, 16, (&arrow[(12 * 2) * 1])); // Arrow DOWN empty
	}else if(arrowState == ARROW_DOWN){
		OLED_drawArea(x, 0, 12, 16, (&arrow[(12 * 2) * 0])); // Arrow UP empty
		OLED_drawArea(x, 24, 12, 16, (&arrow[(12 * 2) * 3])); // Arrow DOWN full
	}else if(arrowState == ARROW_EMPTY){
		OLED_drawArea(x, 0, 12, 16, (&arrow[(12 * 2) * 0])); // Arrow UP empty
		OLED_drawArea(x, 24, 12, 16, (&arrow[(12 * 2) * 1])); // Arrow DOWN empty
	}else{
		// Nothing
	}
	osDelay(50);
}

/**
 * Function to perform jump to system memory boot from user application
 *
 * Call function when you want to jump to system memory
 */
void remove_dfu(void) {

#ifdef DFU_ACTIVATE
	/*
	 * Option bytes programming :
	 * 	After reset, the options related bits in the Flash control register (FLASH_CR) are writeprotected.
	 * 	To run any operation on the option bytes page, the option lock bit OPTLOCK in
	 * 	the Flash control register (FLASH_CR) must be cleared. The following sequence is used to
	 * 	unlock this register:
	 */
    HAL_FLASH_Unlock();

    /*
     * Modifying user options :
     *
     * 		1. Check that no Flash memory operation is on going by checking the BSY bit in the Flash
     *			status register (FLASH_SR).
     *		2. Clear OPTLOCK option lock bit with the clearing sequence described above.
     *		3. Write the desired options value in the options registers
     *		4. Set the Options Start bit OPTSTRT in the Flash control register (FLASH_CR).
     *		5. Wait for the BSY bit to be cleared.
     */
    FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);

    HAL_FLASH_OB_Unlock();

    if(READ_BIT(FLASH->OPTR, FLASH_OPTR_nBOOT1)){
    	CLEAR_BIT(FLASH->OPTR, FLASH_OPTR_nBOOT1);
    	SET_BIT(FLASH->OPTR, FLASH_OPTR_nBOOT0);
    	SET_BIT(FLASH->OPTR, FLASH_OPTR_nSWBOOT0);

        FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);

        SET_BIT(FLASH->CR, FLASH_CR_OPTSTRT);

        FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);

        /*
         * Option byte loading :
         * 		After the BSY bit is cleared, all new options are updated into the flash but they are not
         * 		applied to the system. They will have effect on the system when they are loaded. Option
         * 		bytes loading (OBL) is performed in two cases:
         * 			– When OBL_LAUNCH bit is set in the Flash control register (FLASH_CR).
         * 			– After a power reset (BOR reset or exit from Standby/Shutdown modes).
         */
        HAL_FLASH_OB_Launch();
        HAL_NVIC_SystemReset();
    }

	/* Lock the Flash to disable the flash control register access (recommended
	 * to protect the FLASH memory against possible unwanted operation) ********
	 */
    HAL_FLASH_Lock();
#endif

}

/*
 * Function to check if a new version has just been installed
 */
void software_verification(void){
	if(systemSettings.firmwareMajorVersion != SOFTWARE_MAJOR_VERSION || systemSettings.firmwareMinorVersion != SOFTWARE_MINOR_VERSION || systemSettings.firmwarePatchVersion != SOFTWARE_PATCH_VERSION){
		//New version detected
		OLED_setFont(0);
		OLED_setCursor(0, 8);
		OLED_clearScreen();
		OLED_print(NewFirmware);

		systemSettings.hardwareMajorVersion = HARDWARE_MAJOR_VERSION;
		systemSettings.hardwareMinorVersion = HARDWARE_MINOR_VERSION;
		systemSettings.hardwarePatchVersion = HARDWARE_PATCH_VERSION;

		systemSettings.firmwareMajorVersion = SOFTWARE_MAJOR_VERSION;
		systemSettings.firmwareMinorVersion = SOFTWARE_MINOR_VERSION;
		systemSettings.firmwarePatchVersion = SOFTWARE_PATCH_VERSION;

		saveSettings();

		OLED_refresh();
		osDelay(4000);
	}
}

static void gui_automaticSolderPasteDispenserMode(void) {
	/*
	 * * Automatic Solder Paste Dispenser (gui_automaticSolderPasteDispenserMode)
	 * -> Main loop where we draw the footprint size, and animations
	 * --> User presses buttons and they goto the temperature adjust screen
	 * ---> Display the current setpoint temperature
	 * ---> Use buttons to change forward and back on temperature
	 * ---> Both buttons or timeout for exiting
	 * --> Long hold front button to enter boost mode
	 * ---> Just temporarily sets the system into the alternate temperature for
	 * PID control
	 * --> Long hold back button to exit
	 * --> Double button to exit
	 */
	bool isUP = false;

	for (;;) {

		OLED_clearScreen();

		OLED_setFont(0);

		OLED_setCursor(20, 16);
		OLED_print(SymbolInf);

		OLED_setCursor(95, 16);
		OLED_print(SymbolSup);

		if(aspd100.indexFootPrint >= sizeof(aspd100.footPrints)){
			aspd100.indexFootPrint = 0;
		}

		OLED_setCursor(40, 16);
		OLED_print(aspd100.footPrints[aspd100.indexFootPrint].footprintName);

		ButtonState buttons = getButtonState();
		buttonsSave = buttons;

		switch (buttons) {
			case BUTTON_NONE:
				//displayArrowState(ARROW_EMPTY, 88);
				break;
			case BUTTON_R_LONG:
				break;
			case BUTTON_L_LONG:
				break;
			case BUTTON_UP_LONG:
				break;
			case BUTTON_DOWN_LONG:
				break;
			case BUTTON_CENTER_LONG:
				// Exit this menu
				return;
				break;
			case BUTTON_R_SHORT:
				OLED_setCursor(105, 16);
				OLED_print(SymbolSup);
				if(aspd100.indexFootPrint == sizeof(aspd100.footPrints) - 1){
					aspd100.indexFootPrint = 0;
				}else{
					aspd100.indexFootPrint++;
				}
				osDelay(50);
				break;
			case BUTTON_L_SHORT:
				OLED_setCursor(10, 16);
				OLED_print(SymbolInf);
				if(aspd100.indexFootPrint == 0){
					aspd100.indexFootPrint = sizeof(aspd100.footPrints) - 1;
				}else{
					aspd100.indexFootPrint--;
				}
				osDelay(50);
				break;
			case BUTTON_UP_SHORT:
				/*isUP = true;
				displayArrowState(ARROW_UP, 88);
				drv8876_direction_control(false);*/
				break;
			case BUTTON_DOWN_SHORT:
				/*isUP = false;
				displayArrowState(ARROW_DOWN, 88);
				drv8876_direction_control(true);
				BaseType_t xHigherPriorityTaskWoken = pdFALSE;
				if (motorTaskNotification) {
					vTaskNotifyGiveFromISR(motorTaskNotification,
							&xHigherPriorityTaskWoken);
					portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
				}*/
				break;
			case BUTTON_CENTER_SHORT:

				break;
			default:
				break;
		}

		/*displayArrowState(ARROW_EMPTY, 88);

		if(isUP){
			OLED_setCursor(70, 8);
			OLED_print(UPString);
		}else{
			OLED_setCursor(70, 8);
			OLED_print(DOWNString);
		}*/

		OLED_refresh();

		// Slow down ui update rate
		GUIDelay();
	}
}

void boot_logo_animation(void){
	OLED_clearScreen();

	OLED_setFont(0);


}

/* USER CODE BEGIN Header_StartGUITask */
/**
  * @brief  Function implementing the GUITask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartGUITask */
void StartGUITask(void *argument)
{
  /* USER CODE BEGIN StartGUITask */
  /* Infinite loop */
	OLED_initialize();  // start up the OLED screen

	bool startRootMenu = true;

	boot_logo_animation();

	software_verification();

	remove_dfu(); //Remove DFU if it's necessary

	for (;;) {
		if(startRootMenu){
			enterRootMenu();  // enter the settings menu
		}else{
			gui_automaticSolderPasteDispenserMode();
		}

		startRootMenu = !startRootMenu;

		OLED_refresh();
		osDelay(200);
	}
  /* USER CODE END StartGUITask */
}

