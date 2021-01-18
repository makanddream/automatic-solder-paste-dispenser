/*
 * OLED.hpp
 *
 *  Created on: 28 Sept. 2020
 *      Author: Alan
 *      Designed for the SSD1306
 */

#ifndef OLED_H_
#define OLED_H_
#include <BSP.h>
#include <stdbool.h>
#include <string.h>
#include "I2C_Wrapper.h"
#include "Font.h"
#include "OLED_define.h"
#ifdef __cplusplus
extern "C" {
#endif
#include "FreeRTOS.h"
#ifdef __cplusplus
}
#endif


typedef enum {
	OFF = false, ON = true
} DisplayState;


void OLED_initialize(); // Startup the I2C coms (brings screen out of reset etc)

void OLED_refresh(); // Draw the buffer out to the LCD using the DMA Channel

void OLED_setDisplayState(DisplayState state);

void OLED_setDisplayContrast(uint8_t value);

void OLED_setRotation(bool leftHanded); // Set the rotation for the screen

// Get the current rotation of the LCD
bool OLED_getRotation();

int16_t OLED_getCursorX();

void OLED_print(const char *string);// Draw a string to the current location, with current font

// Set the cursor location by pixels
void OLED_setCursor(int16_t x, int16_t y);

//Set cursor location by chars in current font
void OLED_setCharCursor(int16_t x, int16_t y);

void OLED_setFont(uint8_t fontNumber); // (Future) Set the font that is being used

uint8_t OLED_getFont();

void OLED_drawImage(const uint8_t *buffer, uint8_t x, uint8_t width); // Draws an image to the buffer, at x offset from top to bottom (fixed height renders)

void OLED_printNumber(uint16_t number, uint8_t places, bool noLeaderZeros); // Draws a number at the current cursor location

// Clears the buffer
void OLED_clearScreen();

// Full the buffer
void OLED_fullScreen();

// Draws the battery level symbol
void OLED_drawBattery(uint8_t state);

// Draws a checkbox
void OLED_drawCheckbox(bool state);

void OLED_debugNumber(int32_t val);
void OLED_drawSymbol(uint8_t symbolID);//Used for drawing symbols of a predictable width
void OLED_drawArea(int16_t x, int8_t y, uint8_t wide, uint8_t height,
		const uint8_t *ptr); //Draw an area, but y must be aligned on 0/8 offset
void OLED_drawAreaSwapped(int16_t x, int8_t y, uint8_t wide,
		uint8_t height, const uint8_t *ptr); //Draw an area, but y must be aligned on 0/8 offset
void OLED_fillArea(int16_t x, int8_t y, uint8_t wide, uint8_t height,
		const uint8_t value); //Fill an area, but y must be aligned on 0/8 offset
void OLED_drawFilledRect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,
		bool clear);
void OLED_drawHeatSymbol(uint8_t state);
void OLED_drawScrollIndicator(uint8_t p, uint8_t h); // Draws a scrolling position indicator
void OLED_transitionSecondaryFramebuffer(bool forwardNavigation);
void OLED_useSecondaryFramebuffer(bool useSecondary);

void OLED_drawChar(char c); // Draw a character to a specific location
void OLED_setFramebuffer(uint8_t *buffer);

// Struct to store transformations
typedef struct {
	const uint8_t *currentFont; // Pointer to the current font used for rendering to the buffer
	uint8_t *firstStripPtr; // Pointers to the strips to allow for buffer having extra content
	uint8_t *secondStripPtr;	//Pointers to the strips
	uint8_t *thirdStripPtr;  // The data buffer
	uint8_t *fourthStripPtr;

	bool inLeftHandedMode; // Whether the screen is in left or not (used for offsets in GRAM)
	DisplayState displayState;
	uint8_t displayContrast;
	uint8_t fontWidth, fontHeight;
	int16_t cursor_x, cursor_y;
	uint8_t displayOffset;
	uint8_t screenBuffer[OLED_WIDTH * OLED_HEIGHT / 8 + 30];  // The data buffer
	uint8_t secondFrameBuffer[OLED_WIDTH * OLED_HEIGHT / 8 + 30];
} OLED_t;

#endif /* OLED_H_ */
