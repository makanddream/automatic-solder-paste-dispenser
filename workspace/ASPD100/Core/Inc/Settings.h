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

#define SETTINGSVERSION   ( 0x01 )

#include <stdio.h>
#include <stdbool.h>

/*
 * This struct must be a multiple of 2 bytes as it is saved / restored from
 * flash in uint16_t chunks
 */
typedef struct {
	uint8_t version;   // Used to track if a reset is needed on firmware upgrade


	uint8_t SleepTime;       // minutes timeout to sleep
	uint8_t ShutdownTime;          // Time until unit shuts down if left alone
	uint8_t detailedIDLE :1;       // Detailed idle screen
	uint8_t descriptionScrollSpeed :1;  // Description scroll speed
	uint16_t contrast;  // contrast value
	uint16_t isFirstStart;  // contrast value
	uint8_t modeType;

	uint32_t padding;  // This is here for in case we are not an even divisor so
					   // that nothing gets cut off
					   //MUST BE LAST

} systemSettingsType;

extern volatile systemSettingsType systemSettings;

void saveSettings();
bool restoreSettings();
void resetSettings();

#endif /* INC_SETTINGS_H_ */
