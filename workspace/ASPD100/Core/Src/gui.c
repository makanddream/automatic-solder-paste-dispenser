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


void gui_Menu(const menuitem *menu);

/* Functions of the root menu */
static void displayModeMenu(void);
static bool enterModeMenu(void);
static void displaySettingsMenu(void);
static bool enterSettingsMenu(void);

/* Functions of the mode menu options */
static bool settings_setAutomaticSolderPasteDispenser(void);
static void settings_displayAutomaticSolderPasteDispenser(void);
static bool settings_setVacuumPickUp(void);
static void settings_displayVacuumPickUp(void);

/* Functions of the settings menu options */
static bool settings_setContrast(bool isInc);
static void settings_displayContrast(void);
static bool settings_setScrollSpeed(void);
static void settings_displayScrollSpeed(void);
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

	{ NULL, NULL, NULL }	// end of menu marker. DO NOT REMOVE
};

//const char *tests = test;

menuitem ModeMenu[] = {

	{ (const char*) SettingsDescriptions[0], settings_setAutomaticSolderPasteDispenser, NULL, settings_displayAutomaticSolderPasteDispenser }, /* Automatic solder paste dispenser mode */
	{ (const char*) SettingsDescriptions[1], settings_setVacuumPickUp, NULL, settings_displayVacuumPickUp }, /* Vacuum pick-up mode */

	{ NULL, NULL, NULL }	// end of menu marker. DO NOT REMOVE
};

const menuitem SettingsMenu[] = {
	/*
	 	 	Mode menu options :
	 *		- Automatic solder paste dispenser option
	 *		- Vacuum pick-up mode option
	 */
	{ (const char*) SettingsDescriptions[2], NULL, settings_setContrast, settings_displayContrast }, /* Scroll Speed for descriptions */
	{ (const char*) SettingsDescriptions[3], NULL, settings_setScrollSpeed, settings_displayScrollSpeed }, /* Scroll Speed for descriptions */
	{ (const char*) SettingsDescriptions[4], NULL, settings_setResetSettings, settings_displayResetSettings }, /* Resets settings */

	{ NULL, NULL, NULL }	// end of menu marker. DO NOT REMOVE
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
	OLED_setCharCursor(cursorCharPosition, 0);
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
static bool settings_setAutomaticSolderPasteDispenser(void) {
	return true;
}

static void settings_displayAutomaticSolderPasteDispenser(void) {
	systemSettings.modeType = 1;
	printShortDescription(0, 0);
}

static bool settings_setVacuumPickUp(void) {
	return true;
}

static void settings_displayVacuumPickUp(void) {
	systemSettings.modeType = 2;
	printShortDescription(1, 0);
}
/* end */

/* Start functions of the settings menu options */
static bool settings_setContrast(bool isInc) {
	if(isInc){
		if (systemSettings.contrast > 100) {
			systemSettings.contrast = 0;  // loop back at 255
		} else {
			systemSettings.contrast += 10;  // Go up 10C at a time
		}
	}else{
		if (systemSettings.contrast == 0) {
			systemSettings.contrast = 100;  // loop back at 255
		} else {
			systemSettings.contrast -= 10;  // Go up 10C at a time
		}
	}

	osDelay(25);
	return systemSettings.contrast == 100;
}

static void settings_displayContrast(void) {
	printShortDescription(2, 0);

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

static bool settings_setScrollSpeed(void) {
	if (systemSettings.descriptionScrollSpeed == 0)
		systemSettings.descriptionScrollSpeed = 1;
	else
		systemSettings.descriptionScrollSpeed = 0;
	return false;
}

static void settings_displayScrollSpeed(void) {
	printShortDescription(3, 7);

	displayArrow(85);

	OLED_setCursor(70, 8);
	if(systemSettings.descriptionScrollSpeed){
		OLED_print(FastString);
	}else{
		OLED_print(SlowString);
	}
}

static bool settings_setResetSettings(void) {
	if (userConfirmation(SettingsResetWarning)) {
		resetSettings();

		OLED_setFont(0);
		OLED_setCursor(0, 0);
		OLED_clearScreen();
		OLED_print(ResetOKMessage);
		OLED_refresh();

		waitForButtonPressOrTimeout(2000);  // 2 second timeout
	}
	return false;
}


static void settings_displayResetSettings(void) {
	printShortDescription(4, 0);
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
	gui_Menu(rootMenu);  // Call the root menu
	systemSettings.isFirstStart = 0;
	saveSettings();
}
