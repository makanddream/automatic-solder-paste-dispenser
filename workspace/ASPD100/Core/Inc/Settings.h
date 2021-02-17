/*
 * Settings.h
 *
 *  Created on: Sep 22, 2020
 *      Author: Alan
 *
 *      Houses the system settings and allows saving / restoring from flash
 */

#ifndef INC_SETTINGS_H_
#define INC_SETTINGS_H_

#define SETTINGS_VERSION   ( 0x01 )

#include <stdio.h>
#include <stdbool.h>

/*
 * This struct must be a multiple of 2 bytes as it is saved / restored from
 * flash in uint16_t chunks
 */
typedef struct {
	uint8_t version;   				// Used to track if a reset is needed on firmware upgrade

	/*
	 * The hardware and software version are saved
	 * like this to not use (avoid) float or double type.
	 */
	uint8_t hardwareMajorVersion;	// Store the hardware major version number
	uint8_t hardwareMinorVersion;	// Store the hardware minor version number
	uint8_t hardwarePatchVersion;	// Store the hardware patch version number
	uint8_t firmwareMajorVersion;   // Store the firmware major version number
	uint8_t firmwareMinorVersion;   // Store the firmware minor version number
	uint8_t firmwarePatchVersion;   // Store the firmware patch version number

	uint8_t sleepTime;				// minutes timeout to sleep
	uint8_t contrast;				// contrast value
	uint8_t isFirstStart;			// contrast value
	uint8_t modeType;
	uint8_t descriptionScrollSpeed :1;  // Description scroll speed


	uint32_t padding;  // This is here for in case we are not an even divisor so
					   // that nothing gets cut off
					   //MUST BE LAST

} systemSettingsType;

extern volatile systemSettingsType systemSettings;

void saveSettings();
bool restoreSettings();
void resetSettings();

#endif /* INC_SETTINGS_H_ */
