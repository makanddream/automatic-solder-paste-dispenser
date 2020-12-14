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
	BUTTON_NONE, /* No buttons pressed / < filter time*/
	BUTTON_R_SHORT, /* User has pressed the right button*/
	BUTTON_L_SHORT, /* User has pressed the left button*/
	BUTTON_UP_SHORT, /* User is  holding the up button*/
	BUTTON_DOWN_SHORT, /* User is  holding the down button*/
	BUTTON_CENTER_SHORT, /* User is  holding the center button*/
	BUTTON_R_LONG, /* User is  holding the right button*/
	BUTTON_L_LONG, /* User is  holding the left button*/
	BUTTON_UP_LONG, /* User is  holding the up button*/
	BUTTON_DOWN_LONG, /* User is  holding the down button*/
	BUTTON_CENTER_LONG, /* User is  holding the center button*/

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
