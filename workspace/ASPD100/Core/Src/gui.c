/*
 * gui.c
 *
 *  Created on: Sep 23, 2020
 *      Author: Alan
 */

#include <string.h>
#include "Translation.h"
#include "gui.h"
#include "cmsis_os.h"
#include "main.h"
#include "unit.h"
#include "../../configuration.h"
#include "Buttons.h"

#include "version.h"

void gui_Menu(const menuitem *menu);

/* Functions of the root menu */
static void displayModeMenu(void);
static bool enterModeMenu(void);
static void displaySettingsMenu(void);
static bool enterSettingsMenu(void);

/* Functions of the mode menu options */
static bool settings_setAutomaticFootprints(void);
static void settings_displayAutomaticFootprints(void);
static bool settings_setRefuelSolderPaste(void);
static void settings_displayRefuelSolderPaste(void);
static bool settings_setChangeSyringe(void);
static void settings_displayChangeSyringe(void);

/* Functions of the settings menu options */
static bool settings_setContrast(bool isInc);
static void settings_displayContrast(void);
static bool settings_setLedsBrightness(bool isInc);
static void settings_displayLedsBrightness(void);
static bool settings_setScrollSpeed(void);
static void settings_displayScrollSpeed(void);
static bool settings_setDFU(void);
static void settings_displayDFU(void);
static bool settings_setDeviceInformation(void);
static void settings_displayDeviceInformation(void);
static bool settings_setResetSettings(void);
static void settings_displayResetSettings(void);

/*
 * Root Settings Menu
 *
 * Mode
 *	Automatic solder paste dispenser
 *	Vacuum pick-up
 *
 * Settings
 * 	Contrast
 * 	Unit (metric / imperial)
 * 	Reset Settings
 *
 * Power Saving
 * 	Sleep Time
 * 	Shutdown Time
 * 	Motion Sensitivity
 *
 * UI
 *  // Language
 *  Scrolling Speed
 *  Display orientation
 *
 */
const menuitem rootMenu[] = {
	/*
	 * Mode Menu
	 * Settings Menu
	 * Exit
	 */
	{ (const char*) NULL, enterModeMenu, NULL, displayModeMenu }, /*Mode Menu*/
	{ (const char*) NULL, enterSettingsMenu, NULL, displaySettingsMenu }, /*Settings Menu*/

	{ NULL, NULL, NULL, NULL }	// end of menu marker. DO NOT REMOVE
};

//const char *tests = test;

menuitem ModeMenu[] = {

	{ (const char*) SettingsDescriptions[0], settings_setAutomaticFootprints, NULL, settings_displayAutomaticFootprints }, /* Automatic solder paste dispenser mode */
	{ (const char*) SettingsDescriptions[1], settings_setRefuelSolderPaste, NULL, settings_displayRefuelSolderPaste }, /* Vacuum pick-up mode */
	{ (const char*) SettingsDescriptions[2], settings_setChangeSyringe, NULL, settings_displayChangeSyringe }, /* Change syringe */

	{ NULL, NULL, NULL, NULL }	// end of menu marker. DO NOT REMOVE
};

const menuitem SettingsMenu[] = {
	/*
	 	 	Mode menu options :
	 *		- Automatic solder paste dispenser option
	 *		- Vacuum pick-up mode option
	 */
	{ (const char*) SettingsDescriptions[3], NULL, settings_setContrast, settings_displayContrast }, /* Scroll Speed for descriptions */
	{ (const char*) SettingsDescriptions[4], NULL, settings_setLedsBrightness, settings_displayLedsBrightness }, /* Scroll Speed for descriptions */
	{ (const char*) SettingsDescriptions[5], NULL, settings_setScrollSpeed, settings_displayScrollSpeed }, /* Scroll Speed for descriptions */
	{ (const char*) SettingsDescriptions[6], settings_setDFU, NULL, settings_displayDFU }, /* DFU descriptions */
	{ (const char*) SettingsDescriptions[7], settings_setDeviceInformation, NULL, settings_displayDeviceInformation }, /* Resets settings */
	{ (const char*) SettingsDescriptions[8], settings_setResetSettings, NULL, settings_displayResetSettings }, /* Resets settings */

	{ NULL, NULL, NULL, NULL }	// end of menu marker. DO NOT REMOVE
};

static void printShortDescriptionDoubleLine(uint32_t shortDescIndex) {
	OLED_setFont(1);
	OLED_setCharCursor(0, 1);
	OLED_print(SettingsShortNames[shortDescIndex][0]);
	OLED_setCharCursor(0, 2);
	OLED_print(SettingsShortNames[shortDescIndex][1]);
}

/**
 * Prints two small lines of short description
 * and prepares cursor in big font after it.
 * @param shortDescIndex Index to of short description.
 * @param cursorCharPosition Custom cursor char position to set after printing
 * description.
 */
static void printShortDescription(uint32_t shortDescIndex,
		uint16_t cursorCharPosition) {
	// print short description (default single line, explicit double line)
	printShortDescriptionDoubleLine(shortDescIndex);

	// prepare cursor for value
	OLED_setFont(0);
	OLED_setCharCursor(cursorCharPosition, 8);
	// make room for scroll indicator
	OLED_setCursor(OLED_getCursorX() - 2, 0);
}

static int userConfirmation(const char *message) {
	uint16_t messageWidth = FONT_12_WIDTH * (strlen(message) + 7);
	uint32_t messageStart = xTaskGetTickCount();

	OLED_setFont(0);
	OLED_setCursor(0, 8);
	int16_t lastOffset = -1;
	bool lcdRefresh = true;

	for (;;) {
		int16_t messageOffset = ((xTaskGetTickCount() - messageStart)
				/ 10);
		messageOffset %= messageWidth;  // Roll around at the end

		if (lastOffset != messageOffset) {
			OLED_clearScreen();

			//^ Rolling offset based on time
			OLED_setCursor((OLED_WIDTH - messageOffset), 8);
			OLED_print(message);
			lastOffset = messageOffset;
			lcdRefresh = true;
		}

		ButtonState buttons = getButtonState();
		switch (buttons) {
		case BUTTON_CENTER_SHORT:
			// User confirmed
			return 1;

		case BUTTON_NONE:
		case BUTTON_L_SHORT:
		case BUTTON_R_SHORT:
		case BUTTON_UP_SHORT:
		case BUTTON_DOWN_SHORT:
		case BUTTON_L_LONG:
		case BUTTON_R_LONG:
		case BUTTON_UP_LONG:
		case BUTTON_DOWN_LONG:
			break;
		default:
		case BUTTON_CENTER_LONG:
			return 0;
		}

		if (lcdRefresh) {
			OLED_refresh();
			osDelay(40);
			lcdRefresh = false;
		}
	}
	return 0;
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

/* Functions of the mode menu options */
static bool settings_setAutomaticFootprints(void) {
	return true;
}

static void settings_displayAutomaticFootprints(void) {
	printShortDescription(0, 0);
}

static bool settings_setRefuelSolderPaste(void) {

	bool earlyExit = false;
	uint32_t pwmDutyCycle = stepCounterPeriod / 2; // 50%

	HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
	HAL_NVIC_DisableIRQ(DMA1_Channel2_IRQn);

	tmc2208_change_direction(&tmc2208, E_BACKWARD);

	//Are you sure to start refuel solder paste action ?
	while(earlyExit == false){
		ButtonState buttons = getButtonState();

		switch (buttons) {
			case BUTTON_NONE:
				HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
				break;
			case BUTTON_R_LONG:
				// Number of steps performed by the stepper motor
				if (HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, &pwmDutyCycle, 10000) != HAL_OK)
				{
					/* PWM Generation Error */
					Error_Handler();
				}
				break;
			case BUTTON_L_LONG:
				break;
			case BUTTON_UP_LONG:
				break;
			case BUTTON_DOWN_LONG:
				break;
			case BUTTON_CENTER_LONG:
			case BUTTON_ACTION_LONG:
				// Abort the procedure
				earlyExit = true;
				break;
			case BUTTON_R_SHORT:
				break;
			case BUTTON_L_SHORT:
				break;
			case BUTTON_UP_SHORT:
				break;
			case BUTTON_DOWN_SHORT:
				break;
			case BUTTON_CENTER_SHORT:
				break;
			default:
				break;
		}
	}

	OLED_setFont(0);
	OLED_setCursor(0, 0);
	OLED_clearScreen();
	OLED_print(ResetOKMessage);
	OLED_refresh();

	return false;
}

static void settings_displayRefuelSolderPaste(void) {
	printShortDescription(1, 0);
}

static bool settings_setChangeSyringe(void) {
	/*
	 * Procedure for changing or replacing a syringe
	 *
	 * Step 1 : Start motor in retract mode during 10s
	 * Step 2 : Read the current and verify the motor is not block
	 * Step 3 : If the motor is block print on screen "ERROR PROCEDURE" if not "PROCEDURE DONE"
	 *
	 */
	return false;
}

static void settings_displayChangeSyringe(void) {
	printShortDescription(2, 0);
}
/* end */

/* Start functions of the settings menu options */
static bool settings_setContrast(bool isInc) {
	if(isInc){
		if (systemSettings.contrast == 100) {
			systemSettings.contrast = 10;  // loop back at 100
		} else {
			systemSettings.contrast += 10;  // Go up 10C at a time
		}
	}else{
		if (systemSettings.contrast == 10) {
			systemSettings.contrast = 100;  // loop back at 100
		} else {
			systemSettings.contrast -= 10;  // Go up 10C at a time
		}
	}

	OLED_setDisplayContrast(systemSettings.contrast);

	osDelay(25);
	return systemSettings.contrast == 100;
}

static void settings_displayContrast(void) {
	printShortDescription(3, 0);

	if(systemSettings.contrast == 100){
		displayArrow(83);

		OLED_setCursor(70, 8);
		OLED_printNumber(systemSettings.contrast, 3, true);
	}else{
		displayArrow(88);

		OLED_setCursor(81, 8);
		OLED_printNumber(systemSettings.contrast, 2, true);
	}
	OLED_setCursor(106, 8);
	OLED_print(SymbolPrc);
}

static bool settings_setLedsBrightness(bool isInc) {

	if (systemSettings.ledsBrightness == 1) {
		systemSettings.ledsBrightness = 0;  // loop back at 100
	} else {
		systemSettings.ledsBrightness = 1;  // Go up 10C at a time
	}

	if(systemSettings.ledsBrightness == 1){
		HAL_GPIO_WritePin(LEDs_CTR_GPIO_Port, LEDs_CTR_Pin, GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(LEDs_CTR_GPIO_Port, LEDs_CTR_Pin, GPIO_PIN_RESET);
	}

	osDelay(25);
	return systemSettings.ledsBrightness == 1;
}

static void settings_displayLedsBrightness(void) {
	printShortDescription(4, 0);

	if(systemSettings.ledsBrightness == 1){
		displayArrow(83);

		OLED_setCursor(70, 8);
		OLED_print(OnString);
		//OLED_printNumber(systemSettings.ledsBrightness, 3, true);
	}else{
		displayArrow(88);

		OLED_setCursor(81, 8);
		OLED_print(OffString);
		//OLED_printNumber(systemSettings.ledsBrightness, 2, true);
	}
	OLED_setCursor(106, 8);
	//OLED_print(SymbolPrc);
}

static bool settings_setScrollSpeed(void) {
	if (systemSettings.descriptionScrollSpeed == 0)
		systemSettings.descriptionScrollSpeed = 1;
	else
		systemSettings.descriptionScrollSpeed = 0;
	return false;
}

static void settings_displayScrollSpeed(void) {
	printShortDescription(5, 7);

	displayArrow(85);

	OLED_setCursor(70, 8);
	if(systemSettings.descriptionScrollSpeed){
		OLED_print(FastString);
	}else{
		OLED_print(SlowString);
	}
}

static bool settings_setDFU(void) {

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

    SET_BIT(FLASH->OPTR, FLASH_OPTR_nBOOT1);
    CLEAR_BIT(FLASH->OPTR, FLASH_OPTR_nBOOT0);
    CLEAR_BIT(FLASH->OPTR, FLASH_OPTR_nSWBOOT0);

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
#endif

	return false;
}

static void settings_displayDFU(void) {
	printShortDescription(6, 0);
}

static bool settings_setDeviceInformation(void) {

	OLED_clearScreen();
	OLED_setFont(1);

	/*
	 * Print hardware version
	 */
	OLED_setCharCursor(0, 1);
	OLED_print(HardwareVersion);
	OLED_setCharCursor(13, 1);
	OLED_printNumber(HARDWARE_MAJOR_VERSION, 1, true);
	OLED_setCharCursor(14, 1);
	OLED_print(SymbolDot);
	OLED_setCharCursor(15, 1);
	OLED_printNumber(HARDWARE_MINOR_VERSION, 1, true);
	OLED_setCharCursor(16, 1);
	OLED_print(SymbolDot);
	OLED_setCharCursor(17, 1);
	OLED_printNumber(HARDWARE_PATCH_VERSION, 1, true);

	/*
	 * Print software version
	 */
	OLED_setCharCursor(0, 3);
	OLED_print(SoftwareVersion);
	OLED_setCharCursor(13, 3);
	OLED_printNumber(SOFTWARE_MAJOR_VERSION, 1, true);
	OLED_setCharCursor(14, 3);
	OLED_print(SymbolDot);
	OLED_setCharCursor(15, 3);
	OLED_printNumber(SOFTWARE_MINOR_VERSION, 1, true);
	OLED_setCharCursor(16, 3);
	OLED_print(SymbolDot);
	OLED_setCharCursor(17, 3);
	OLED_printNumber(SOFTWARE_PATCH_VERSION, 1, true);

	OLED_refresh();

	osDelay(25);

	waitForButtonPress();

	return false;
}


static void settings_displayDeviceInformation(void) {
	printShortDescription(7, 0);
}

static bool settings_setResetSettings(void) {
	if (userConfirmation(SettingsResetWarning)) {
		resetSettings();

		OLED_setFont(0);
		OLED_setCursor(20, 8);
		OLED_clearScreen();
		OLED_print(ResetOKMessage);
		OLED_refresh();

		osDelay(25);

		waitForButtonPressOrTimeout(2000);  // 2 second timeout
	}
	return false;
}


static void settings_displayResetSettings(void) {
	printShortDescription(8, 0);
}
/* end */

static void displayMenu(size_t index) {
// Call into the menu
	OLED_setFont(0);
	OLED_setCursor(0, 8);
// Draw title
	OLED_print(RootMenuEntries[index]);
// Draw symbol
// 16 pixel wide image
// 2 pixel wide scrolling indicator
	OLED_drawArea(OLED_WIDTH - 32, 0, 32, 32, (&SettingsMenuIcons[(64 * 2) * index]));
}

/* Functions of the root menu */
static void displayModeMenu(void) {
	displayMenu(0);
}
static bool enterModeMenu(void) {
	gui_Menu(ModeMenu);
	return false;
}
static void displaySettingsMenu(void) {
	displayMenu(1);
}
static bool enterSettingsMenu(void) {
	gui_Menu(SettingsMenu);
	return false;
}
/* end */

void gui_Menu(const menuitem *menu) {
// Draw the settings menu and provide iteration support etc
	uint8_t currentScreen = 0;
	uint32_t autoRepeatTimer = 0;
	uint8_t autoRepeatAcceleration = 0;
	bool earlyExit = false;
	uint32_t descriptionStart = 0;
	int16_t lastOffset = -1;
	bool lcdRefresh = true;
	ButtonState lastButtonState = BUTTON_NONE;
	static bool enterGUIMenu = true;
	enterGUIMenu = true;
	uint8_t scrollContentSize = 0;
	bool lastValue = false;
	bool isSelect = false;

	for (uint8_t i = 0; menu[i].draw != NULL; i++) {
		scrollContentSize += 1;
	}

	// Animated menu opening.
	if (menu[currentScreen].draw != NULL) {
		// This menu is drawn in a secondary framebuffer.
		// Then we play a transition from the current primary
		// framebuffer to the new buffer.
		// The extra buffer is discarded at the end of the transition.
		OLED_useSecondaryFramebuffer(true);
		OLED_setFont(0);
		OLED_setCursor(0, 0);
		OLED_clearScreen();
		menu[currentScreen].draw();
		OLED_useSecondaryFramebuffer(false);
		OLED_transitionSecondaryFramebuffer(true);
	}

	while ((menu[currentScreen].draw != NULL) && earlyExit == false) {
		OLED_setFont(0);
		OLED_setCursor(0, 0);
		// If the user has hesitated for >=3 seconds, show the long text
		// Otherwise "draw" the option
		if ((xTaskGetTickCount() - lastButtonTime < 3000)
				|| menu[currentScreen].description == NULL) {
			OLED_clearScreen();
			menu[currentScreen].draw();
			lcdRefresh = true;
		} else {
			// Draw description
			if (descriptionStart == 0)
				descriptionStart = xTaskGetTickCount();
			// lower the value - higher the speed
			int16_t descriptionWidth =
			FONT_12_WIDTH * (strlen(menu[currentScreen].description) + 7);
			int16_t descriptionOffset =
					((xTaskGetTickCount() - descriptionStart)
							/ (systemSettings.descriptionScrollSpeed == 1 ?
									10 : 20));
			descriptionOffset %= descriptionWidth;	// Roll around at the end
			if (lastOffset != descriptionOffset) {
				OLED_clearScreen();
				OLED_setCursor((OLED_WIDTH - descriptionOffset), 8);
				OLED_print(menu[currentScreen].description);
				lastOffset = descriptionOffset;
				lcdRefresh = true;
			}
		}

		ButtonState buttons = getButtonState();
		buttonsSave = buttons;

		if (buttons != lastButtonState) {
			lastButtonState = buttons;
		}

		switch (buttons) {
			case BUTTON_R_SHORT:
				// increment
				if (descriptionStart == 0) {
					if(currentScreen == (scrollContentSize - 1)){
						currentScreen = 0;
					}else{
						currentScreen++;
						lastValue = false;
					}
				} else
					descriptionStart = 0;
				break;
			case BUTTON_L_SHORT:
				if (descriptionStart == 0) {
					if(currentScreen == 0){
						currentScreen = scrollContentSize - 1;
					}else{
						currentScreen--;
						lastValue = false;
					}
				} else
					descriptionStart = 0;
				break;
			case BUTTON_UP_SHORT:
				if (menu[currentScreen].incrementHandler != NULL){
					lastValue = menu[currentScreen].incrementHandler(true);
				}
				break;
			case BUTTON_UP_LONG:
				break;
			case BUTTON_DOWN_SHORT:
				if (menu[currentScreen].incrementHandler != NULL){
					lastValue = menu[currentScreen].incrementHandler(false);
				}
				break;
			case BUTTON_DOWN_LONG:
				break;
			case BUTTON_CENTER_SHORT:
			case BUTTON_ACTION_SHORT:
				if (descriptionStart == 0) {
					if (menu[currentScreen].validateHandler != NULL) {
						enterGUIMenu = false;
						lastValue = menu[currentScreen].validateHandler();

						if (enterGUIMenu) {
							OLED_useSecondaryFramebuffer(true);
							OLED_setFont(0);
							OLED_setCursor(0, 0);
							OLED_clearScreen();
							menu[currentScreen].draw();
							OLED_useSecondaryFramebuffer(false);
							OLED_transitionSecondaryFramebuffer(false);
						}
						enterGUIMenu = true;
					} else {
						earlyExit = true;
					}
				} else
					descriptionStart = 0;
				isSelect = true;
				break;
			case BUTTON_CENTER_LONG:
				earlyExit = true;
				break;
			case BUTTON_NONE:
			default:
				break;
		}

		if ((PRESS_ACCEL_INTERVAL_MAX - autoRepeatAcceleration) <
		PRESS_ACCEL_INTERVAL_MIN) {
			autoRepeatAcceleration =
			PRESS_ACCEL_INTERVAL_MAX - PRESS_ACCEL_INTERVAL_MIN;
		}

		if (lcdRefresh) {
			OLED_refresh();  // update the OLED screen
			osDelay(40);
			lcdRefresh = false;
		}
		if ((xTaskGetTickCount() - lastButtonTime) > (1000 * 30)) {
			// If user has not pressed any buttons in 30 seconds, exit back a menu layer
			// This will trickle the user back to the main screen eventually
			//earlyExit = true;
			//descriptionStart = 0;
		}
	}
}

void enterRootMenu() {
	HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
	gui_Menu(rootMenu);  // Call the root menu
	systemSettings.isFirstStart = 0;
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
	saveSettings();
}
