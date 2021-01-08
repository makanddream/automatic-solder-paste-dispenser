/*
 * gui.h
 *
 *  Created on: Sep 23, 2020
 *      Author: Alan
 */

#ifndef INC_GUI_H_
#define INC_GUI_H_
#include "Translation.h"
#include "Settings.h"

#define PRESS_ACCEL_STEP			30
#define PRESS_ACCEL_INTERVAL_MIN	100
#define PRESS_ACCEL_INTERVAL_MAX	300

//GUI holds the menu structure and all its methods for the menu itself

//Declarations for all the methods for the settings menu (at end of this file)

//Struct for holding the function pointers and descriptions
typedef struct {
	const char *description;
	// return true if increment reached the maximum value
	bool (* const validateHandler)(void);
	bool (* const incrementHandler)(bool);
	void (* const draw)(void);
} menuitem;

void enterRootMenu();
void GUIDelay();
extern const menuitem rootMenu[];


#endif /* INC_GUI_H_ */
