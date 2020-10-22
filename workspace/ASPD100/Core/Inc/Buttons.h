/*
 * Buttons.h
 *
 *  Created on: 21 sept. 2020
 *      Author: Alan
 */

#ifndef INC_BUTTONS_H_
#define INC_BUTTONS_H_

#include <stdio.h>
#include "BSP.h"

extern uint32_t lastButtonTime;

typedef enum ButtonState_t {
	BUTTON_NONE = 0, /* No buttons pressed / < filter time*/
	BUTTON_R_SHORT = 1, /* User has pressed the front button*/
	BUTTON_L_SHORT = 2, /* User has pressed the back  button*/
	BUTTON_R_LONG = 4, /* User is  holding the front button*/
	BUTTON_L_LONG = 8, /* User is  holding the back button*/
	BUTTON_BOTH = 16, /* User has pressed both buttons*/

/*
 * Note:
 * Pressed means press + release, we trigger on a full \__/ pulse
 * holding means it has gone low, and been low for longer than filter time
 */
}ButtonState;

//Returns what buttons are pressed (if any)
ButtonState getButtonState();
//Helpers
void waitForButtonPressOrTimeout(uint32_t timeout);
void waitForButtonPress();

#endif /* INC_BUTTONS_H_ */
