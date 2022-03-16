/*
 * Settings.c
 *
 *  Created on: Sep 22, 2020
 *      Author: Alan
 *
 *      This file holds the users settings and saves / restores them to the
 * devices flash
 */

#include "Settings.h"
//#include "Setup.h"
#include "../../configuration.h"
#include "BSP.h"

#include "version.h"

#include "string.h"
volatile systemSettingsType systemSettings;

void saveSettings() {
#ifdef SAVE_SETTINGS
	// First we erase the flash
	flash_save_buffer((uint8_t*) &systemSettings, sizeof(systemSettingsType));
#endif
}

bool restoreSettings() {
	// We read the flash
	flash_read_buffer((uint8_t*) &systemSettings, sizeof(systemSettingsType));

	// if the version is correct were done
	// if not we reset and save
	if (systemSettings.version != SETTINGS_VERSION) {
		// probably not setup
		resetSettings();
		return true;
	}
	return false;
}

void resetSettings() {
	memset((void*) &systemSettings, 0, sizeof(systemSettingsType));

	systemSettings.version =					SETTINGS_VERSION;	// Store the version number to allow for easier upgrades

	systemSettings.hardwareMajorVersion =		HARDWARE_MAJOR_VERSION;	// Store the hardware major version number
	systemSettings.hardwareMinorVersion =		HARDWARE_MINOR_VERSION;	// Store the hardware minor version number
	systemSettings.hardwarePatchVersion =		HARDWARE_PATCH_VERSION;	// Store the hardware patch version number

	systemSettings.firmwareMajorVersion =		SOFTWARE_MAJOR_VERSION;	// Store the firmware major version number
	systemSettings.firmwareMinorVersion =		SOFTWARE_MINOR_VERSION;	// Store the firmware major version number
	systemSettings.firmwarePatchVersion =		SOFTWARE_PATCH_VERSION;	// Store the firmware major version number

	systemSettings.sleepTime =					SLEEP_TIME; 		// How many seconds/minutes we wait until going
	systemSettings.contrast =					CONTRAST_VALUE;		// The default contrast is 50%
	systemSettings.ledsBrightness = 			LEDS_BRIGHTNESS;	// The default leds brightness is 50%
	systemSettings.isFirstStart =				FIRST_START;		// TODO
	systemSettings.modeType =					DEFAULT_MODE;		// TODO

	systemSettings.descriptionScrollSpeed = 	DESCRIPTION_SCROLL_SPEED; // Default to slow

	saveSettings();  // Save defaults
}
