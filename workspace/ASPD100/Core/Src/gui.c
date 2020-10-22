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
static bool settings_setContrast(void);
static void settings_displayContrast(void);
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
	{ (const char*) NULL, enterModeMenu, displayModeMenu }, /*Mode Menu*/
	{ (const char*) NULL, enterSettingsMenu, displaySettingsMenu }, /*Settings Menu*/

	{ NULL, NULL, NULL }	// end of menu marker. DO NOT REMOVE
};

const menuitem ModeMenu[] = {
	/*
	 * 	Mode menu options :
	 *		- Automatic solder paste dispenser option
	 *		- Vacuum pick-up mode option
	 */
	{ (const char*) &SettingsDescriptions[8], settings_setAutomaticSolderPasteDispenser, settings_displayAutomaticSolderPasteDispenser }, /* Automatic solder paste dispenser mode */
	{ (const char*) &SettingsDescriptions[9], settings_setVacuumPickUp, settings_displayVacuumPickUp }, /* Vacuum pick-up mode */

	{ NULL, NULL, NULL }	// end of menu marker. DO NOT REMOVE
};

const menuitem SettingsMenu[] = {
	/*
	 	 	Mode menu options :
	 *		- Automatic solder paste dispenser option
	 *		- Vacuum pick-up mode option
	 */
	{ (const char*) &SettingsDescriptions[15], settings_setContrast, settings_displayContrast }, /* Scroll Speed for descriptions */
	{ (const char*) &SettingsDescriptions[15], settings_setResetSettings, settings_displayResetSettings }, /* Scroll Speed for descriptions */

	{ NULL, NULL, NULL }	// end of menu marker. DO NOT REMOVE
};

static void printShortDescriptionDoubleLine(uint32_t shortDescIndex) {
	OLED_setFont(1);
	OLED_setCharCursor(0, 0);
	OLED_print(SettingsShortNames[shortDescIndex][0]);
	OLED_setCharCursor(0, 4);
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
	OLED_setCursor(0, 0);
	int16_t lastOffset = -1;
	bool lcdRefresh = true;

	for (;;) {
		int16_t messageOffset = ((xTaskGetTickCount() - messageStart)
				/ 10);
		messageOffset %= messageWidth;  // Roll around at the end

		if (lastOffset != messageOffset) {
			OLED_clearScreen();

			//^ Rolling offset based on time
			OLED_setCursor((OLED_WIDTH - messageOffset), 0);
			OLED_print(message);
			lastOffset = messageOffset;
			lcdRefresh = true;
		}

		ButtonState buttons = getButtonState();
		switch (buttons) {
		case BUTTON_R_SHORT:
			// User confirmed
			return 1;

		case BUTTON_NONE:
		case BUTTON_L_LONG:
		case BUTTON_R_LONG:
			break;
		default:
		case BUTTON_BOTH:
		case BUTTON_L_SHORT:
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
static bool settings_setContrast(void) {
	return true;
}

static void settings_displayContrast(void) {
	printShortDescription(2, 0);
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
	printShortDescription(3, 0);
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
		//OLED_setFont(0);
		//OLED_setCursor(0, 0);
		// If the user has hesitated for >=3 seconds, show the long text
		// Otherwise "draw" the option
		if ((xTaskGetTickCount() - lastButtonTime < 3000)
				) {
			OLED_clearScreen();
			menu[currentScreen].draw();
			lcdRefresh = true;
		} else {
			// Draw description
			if (descriptionStart == 0)
				descriptionStart = xTaskGetTickCount();
			// lower the value - higher the speed
			/*int16_t descriptionWidth =
			FONT_12_WIDTH * (strlen(menu[currentScreen].description) + 7);
			int16_t descriptionOffset =
					((xTaskGetTickCount() - descriptionStart)
							/ 10);//(systemSettings.descriptionScrollSpeed == 1 ? 10 : 20));
			descriptionOffset %= descriptionWidth;	// Roll around at the end
			if (lastOffset != descriptionOffset) {
				OLED_clearScreen();
				OLED_setCursor((OLED_WIDTH - descriptionOffset), 0);
				OLED_print(menu[currentScreen].description);
				OLED_print(SleepingAdvancedString);
				lastOffset = descriptionOffset;
				lcdRefresh = true;
			}*/
		}

		ButtonState buttons = getButtonState();

		if (buttons != lastButtonState) {
			lastButtonState = buttons;
		}

		switch (buttons) {
			case BUTTON_BOTH:
				//saveSettings();
				break;
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
			case BUTTON_R_LONG:
				if (descriptionStart == 0) {
					if (menu[currentScreen].incrementHandler != NULL) {
						enterGUIMenu = false;
						lastValue = menu[currentScreen].incrementHandler();

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
			case BUTTON_L_LONG:
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
