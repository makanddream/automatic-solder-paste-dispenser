/*
 * OLED.c
 *
 *  Created on: 28 Sept. 2020
 *      Author: Alan
 */

#include "OLED.h"

#include <string.h>
#include <stdlib.h>
#include "Translation.h"
#include "cmsis_os.h"
#include "../../configuration.h"

OLED_t OLED;

/*Setup params for the OLED screen*/
/* ../docs/datasheet/SSD1306_v1.1.pdf */
/*All commands are prefixed with 0x80*/
/*Data packets are prefixed with 0x40*/

uint8_t OLED_Setup_Array[] = {
	/**/
	CMD_TYPE, CMD_DISPLAY_OFF, 								/* Display off */
	CMD_TYPE, 0xD5, 										/* Set display clock divide ratio / osc freq */
	//CMD_TYPE, 0x52, /* Divide ratios */
	CMD_TYPE, 0xF0, 										/* Divide ratios */
	CMD_TYPE, 0xA8, 										/* Set Multiplex Ratio */
	//CMD_TYPE, 0x0F, /* 16 == max brightness,39==dimmest */
	CMD_TYPE, 0x3F, 										/* 16 == max brightness,39==dimmest */
	CMD_TYPE, 0xC0, 										/* Set COM Scan direction */
	CMD_TYPE, CMD_SET_DISPLAY_OFFSET, CMD_TYPE, 0x00, 		/* Set vertical Display offset to 0 */
	CMD_TYPE, 0x40, 										/* Set Display start line to 0 */
	CMD_TYPE, 0xA0, 										/* Set Segment remap to normal */
	CMD_TYPE, 0x8D, 										/* Charge Pump */
	CMD_TYPE, 0x14, 										/* Charge Pump settings */
	CMD_TYPE, CMD_SET_COM_PINS_HW_CONFIG, CMD_TYPE, 0x02, 	/*Set COM Pins Hardware Config for 128x32 is 0x02 */
	CMD_TYPE, CMD_SET_CONTRAST_CONTROL, CMD_TYPE, 0x7F, 	/* Contrast to 100% */
	CMD_TYPE, 0xD9, 										/* Set pre-charge period */
	CMD_TYPE, 0xF1, 										/* Pre charge period */
	//CMD_TYPE, 0xF1, /* Pre charge period */
	CMD_TYPE, 0x22, 										/* Pre charge period */
	CMD_TYPE, 0xDB, 										/* Adjust VCOMH regulator output */
	CMD_TYPE, 0x30, 										/* VCOM level */
	CMD_TYPE, CMD_RESUME_DISPLAY_TO_GDDRAM, 				/* Enable the display GDDR */
	CMD_TYPE, CMD_SET_NORMAL_DISPLAY, 						/* Normal display */
	CMD_TYPE, CMD_SET_MEMORY_ADDRESSING_MODE, CMD_TYPE, 0x00, /* Memory Mode to horizontal addressing mode*/
	CMD_TYPE, CMD_DISPLAY_ON 								/* Display on */
};


const uint8_t REFRESH_COMMANDS[] = {
	CMD_TYPE, CMD_DISPLAY_ON, /* Display on */
	CMD_TYPE, CMD_SET_COLUMN_ADDRESS, CMD_TYPE, 0x00, CMD_TYPE, 0x7F, 	/* Setup column start and end address */
	CMD_TYPE, CMD_SET_MEMORY_ADDRESSING_MODE, CMD_TYPE, 0x00, 			/* Set Memory Addressing Mode to Horizontal Addressing Mode */
	CMD_TYPE, CMD_SET_PAGE_ADDRESS, CMD_TYPE, 0x00, CMD_TYPE, 0x03, 	/* Setup page start and end address */
	DATA_TYPE /* This specify the next data is for the GGDRAM (Graphic Display Data RAM) */
};

/*
 * Animation timing function that follows a Bezier curve.
 * @param t A given percentage value [0..<100]
 * Returns a new percentage value with ease in and ease out.
 * Original floating point formula: t * t * (3.0f - 2.0f * t);
 */
static uint8_t easeInOutTiming(uint8_t t) {
	return t * t * (300 - 2 * t) / 10000;
}

/*
 * Returns the value between a and b, using a percentage value t.
 * @param a The value associated with 0%
 * @param b The value associated with 100%
 * @param t The percentage [0..<100]
 */
static uint8_t lerp(uint8_t a, uint8_t b, uint8_t t) {
	return a + t * (b - a) / 100;
}

void OLED_initialize() {

	OLED.cursor_x = OLED.cursor_y = 0;
	OLED.currentFont = USER_FONT_6x8;
	OLED.fontWidth = 6;
	OLED.inLeftHandedMode = false;
	OLED.firstStripPtr = &OLED.screenBuffer[sizeof(REFRESH_COMMANDS)];
	OLED.secondStripPtr = &OLED.screenBuffer[sizeof(REFRESH_COMMANDS) + OLED_WIDTH];
	OLED.thirdStripPtr = &OLED.screenBuffer[sizeof(REFRESH_COMMANDS) + (OLED_WIDTH * 2)];
	OLED.fourthStripPtr = &OLED.screenBuffer[sizeof(REFRESH_COMMANDS) + (OLED_WIDTH * 3)];
	OLED.fontHeight = 8;
	OLED.displayOffset = 0;
	memcpy(&OLED.screenBuffer[0], &REFRESH_COMMANDS[0], sizeof(REFRESH_COMMANDS));

	// Set the display to be ON once the settings block is sent and send the
	// Initialization data to the OLED.

	OLED_setDisplayState(ON);
	FRToSI2C_Transmit(DEVICEADDR_OLED, &OLED_Setup_Array[0],
			sizeof(OLED_Setup_Array));
}

// Draw the buffer out to the LCD using the DMA Channel
void OLED_refresh() {
	FRToSI2C_Transmit(DEVICEADDR_OLED, OLED.screenBuffer, sizeof(REFRESH_COMMANDS) + (OLED_WIDTH * OLED_HEIGHT / 8));
	//DMA tx time is ~ 20mS Ensure after calling this you delay for at least 25ms
	//or we need to goto double buffering
}

void OLED_setDisplayState(DisplayState state) {
	OLED.displayState = state;
	OLED.screenBuffer[1] = (state == ON) ? CMD_DISPLAY_ON : CMD_DISPLAY_OFF;
}

// Get the current rotation of the LCD
bool OLED_getRotation() {
	return OLED.inLeftHandedMode;
}

int16_t OLED_getCursorX() {
	return OLED.cursor_x;
}

// Set the cursor location by pixels
void OLED_setCursor(int16_t x, int16_t y) {
	OLED.cursor_x = x;
	OLED.cursor_y = y;
}

//Set cursor location by chars in current font
void OLED_setCharCursor(int16_t x, int16_t y) {
	OLED.cursor_x = x * OLED.fontWidth;
	OLED.cursor_y = y * OLED.fontHeight;
}

void OLED_drawImage(const uint8_t *buffer, uint8_t x, uint8_t width) {
	OLED_drawArea(x, 0, width, 16, buffer);
}

// Clears the buffer
void OLED_clearScreen() {
	memset(OLED.firstStripPtr, 0x00, (OLED_WIDTH * OLED_HEIGHT / 8));
}

// Clears the buffer
void OLED_fullScreen() {
	memset(OLED.firstStripPtr, 0xFF, (OLED_WIDTH * OLED_HEIGHT / 8));
}

// Draws the battery level symbol
void OLED_drawBattery(uint8_t state) {
	OLED_drawSymbol(3 + (state > 10 ? 10 : state));
}

// Draws a checkbox
void OLED_drawCheckbox(bool state) {
	OLED_drawSymbol((state) ? 16 : 17);
}


void OLED_setFramebuffer(uint8_t *buffer) {
	if (buffer == NULL) {
		OLED.firstStripPtr = &OLED.screenBuffer[sizeof(REFRESH_COMMANDS)];
		OLED.secondStripPtr = &OLED.screenBuffer[sizeof(REFRESH_COMMANDS) + OLED_WIDTH];
		OLED.thirdStripPtr = &OLED.screenBuffer[sizeof(REFRESH_COMMANDS) + OLED_WIDTH*2];
		OLED.fourthStripPtr = &OLED.screenBuffer[sizeof(REFRESH_COMMANDS) + OLED_WIDTH*3];
		return;
	}

	OLED.firstStripPtr = &buffer[0];
	OLED.secondStripPtr = &buffer[OLED_WIDTH];
	OLED.thirdStripPtr = &buffer[OLED_WIDTH*2];
	OLED.fourthStripPtr = &buffer[OLED_WIDTH*3];
}

/*
 * Prints a char to the screen.
 * UTF font handling is done using the two input chars.
 * Precursor is the command char that is used to select the table.
 */
void OLED_drawChar(char c) {
	if (c == '\x01' && OLED.cursor_y == 0) { // 0x01 is used as new line char
		OLED.cursor_x = 0;
		OLED.cursor_y = 8;
		return;
	} else if (c == 0) {
		return;
	}
	uint16_t index = c - 2; //First index is \x02
	uint8_t *charPointer;
	charPointer = ((uint8_t*) OLED.currentFont)
			+ ((OLED.fontWidth * (OLED.fontHeight / 8)) * index);
	OLED_drawArea(OLED.cursor_x, OLED.cursor_y, OLED.fontWidth, OLED.fontHeight, charPointer);
	OLED.cursor_x += OLED.fontWidth;
}

/*
 * Draws a one pixel wide scrolling indicator. y is the upper vertical position
 * of the indicator in pixels (0..<16).
 */
void OLED_drawScrollIndicator(uint8_t y, uint8_t height) {
	union u_type {
		uint16_t whole;
		uint8_t strips[2];
	} column;

	column.whole = (1 << height) - 1;
	column.whole <<= y;

	// Draw a one pixel wide bar to the left with a single pixel as
	// the scroll indicator.
	OLED_fillArea(OLED_WIDTH - 1, 0, 1, 8, column.strips[0]);
	OLED_fillArea(OLED_WIDTH - 1, 8, 1, 8, column.strips[1]);
}

/**
 * Plays a transition animation between two framebuffers.
 * @param forwardNavigation Direction of the navigation animation.
 *
 * If forward is true, this displays a forward navigation to the second framebuffer contents.
 * Otherwise a rewinding navigation animation is shown to the second framebuffer contents.
 */
void OLED_transitionSecondaryFramebuffer(bool forwardNavigation) {
	uint8_t *firstBackStripPtr = &OLED.secondFrameBuffer[0];
	uint8_t *secondBackStripPtr = &OLED.secondFrameBuffer[OLED_WIDTH];
	uint8_t *thirdBackStripPtr = &OLED.secondFrameBuffer[OLED_WIDTH*2];
	uint8_t *fourthBackStripPtr = &OLED.secondFrameBuffer[OLED_WIDTH*3];

	uint32_t totalDuration = 70; // 500ms
	uint32_t duration = 0;
	uint32_t start = xTaskGetTickCount();
	uint8_t offset = 0;

	while (duration <= totalDuration) {
		duration = xTaskGetTickCount() - start;
		uint8_t progress = duration * 1000 / totalDuration;
		progress = easeInOutTiming(progress);
		progress = lerp(0, OLED_WIDTH, progress);
		if (progress > OLED_WIDTH) {
			progress = OLED_WIDTH;
		}

		// When forward, current contents move to the left out.
		// Otherwise the contents move to the right out.
		uint8_t oldStart = forwardNavigation ? 0 : progress;
		uint8_t oldPrevious = forwardNavigation ? progress - offset : offset;

		// Content from the second framebuffer moves in from the right (forward)
		// or from the left (not forward).
		uint8_t newStart = forwardNavigation ? OLED_WIDTH - progress : 0;
		uint8_t newEnd = forwardNavigation ? 0 : OLED_WIDTH - progress;

		offset = progress;

		memmove(&OLED.firstStripPtr[oldStart], &OLED.firstStripPtr[oldPrevious],
		OLED_WIDTH - progress);
		memmove(&OLED.secondStripPtr[oldStart], &OLED.secondStripPtr[oldPrevious],
		OLED_WIDTH - progress);
		memmove(&OLED.thirdStripPtr[oldStart], &OLED.thirdStripPtr[oldPrevious],
		OLED_WIDTH - progress);
		memmove(&OLED.fourthStripPtr[oldStart], &OLED.fourthStripPtr[oldPrevious],
		OLED_WIDTH - progress);

		memmove(&OLED.firstStripPtr[newStart], &firstBackStripPtr[newEnd], progress);
		memmove(&OLED.secondStripPtr[newStart], &secondBackStripPtr[newEnd], progress);
		memmove(&OLED.thirdStripPtr[newStart], &thirdBackStripPtr[newEnd], progress);
		memmove(&OLED.fourthStripPtr[newStart], &fourthBackStripPtr[newEnd], progress);

		OLED_refresh();
		osDelay(40);
	}
}

void OLED_useSecondaryFramebuffer(bool useSecondary) {
	if (useSecondary) {
		OLED_setFramebuffer(OLED.secondFrameBuffer);
	} else {
		OLED_setFramebuffer(NULL);
	}
}

void OLED_setRotation(bool leftHanded) {
#ifdef OLED_FLIP
	leftHanded = !leftHanded;
#endif
	if (OLED.inLeftHandedMode == leftHanded) {
		return;
	}

	// send command struct again with changes
	if (leftHanded) {
		OLED_Setup_Array[11] = 0xC8;  // c1?
		OLED_Setup_Array[19] = 0xA1;
	} else {
		OLED_Setup_Array[11] = 0xC0;
		OLED_Setup_Array[19] = 0xA0;
	}
	FRToSI2C_Transmit(DEVICEADDR_OLED, (uint8_t*) OLED_Setup_Array,
			sizeof(OLED_Setup_Array));
	OLED.inLeftHandedMode = leftHanded;

	OLED.screenBuffer[5] = OLED.inLeftHandedMode ? 0 : 32; // display is shifted by 32 in left handed
												 // mode as driver ram is 128 wide
	OLED.screenBuffer[7] = OLED.inLeftHandedMode ? 95 : 0x7F; // End address of the ram segment we are writing to (OLED_WIDTH wide)
	OLED.screenBuffer[9] = OLED.inLeftHandedMode ? 0xC8 : 0xC0;
}

// print a string to the current cursor location
void OLED_print(const char *str) {
	while (str[0]) {
		OLED_drawChar(str[0]);
		str++;
	}
}

void OLED_setFont(uint8_t fontNumber) {
	if (fontNumber == 1) {
		// small font
		OLED.currentFont = USER_FONT_6x8;
		OLED.fontHeight = 8;
		OLED.fontWidth = 6;
	} else if (fontNumber == 2) {
		OLED.currentFont = ExtraFontChars;
		OLED.fontHeight = 16;
		OLED.fontWidth = 12;
	} else {
		OLED.currentFont = USER_FONT_12x16;
		OLED.fontHeight = 16;
		OLED.fontWidth = 12;
	}
}
uint8_t OLED_getFont() {
	if (OLED.currentFont == USER_FONT_6x8)
		return 1;
	else if (OLED.currentFont == ExtraFontChars)
		return 2;
	else
		return 0;
}
inline void stripLeaderZeros(char *buffer, uint8_t places) {
	//Removing the leading zero's by swapping them to SymbolSpace
	// Stop 1 short so that we dont blank entire number if its zero
	for (int i = 0; i < (places - 1); i++) {
		if (buffer[i] == 2) {
			buffer[i] = SymbolSpace[0];
		} else {
			return;
		}
	}
}
// maximum places is 5
void OLED_printNumber(uint16_t number, uint8_t places, bool noLeaderZeros) {
	char buffer[7] = { 0 };

	if (places >= 5) {
		buffer[5] = 2 + number % 10;
		number /= 10;
	}
	if (places > 4) {
		buffer[4] = 2 + number % 10;
		number /= 10;
	}

	if (places > 3) {
		buffer[3] = 2 + number % 10;
		number /= 10;
	}

	if (places > 2) {
		buffer[2] = 2 + number % 10;
		number /= 10;
	}

	if (places > 1) {
		buffer[1] = 2 + number % 10;
		number /= 10;
	}

	buffer[0] = 2 + number % 10;
	if (noLeaderZeros)
		//stripLeaderZeros(buffer, places);
	OLED_print(buffer);
}

void OLED_debugNumber(int32_t val) {
	if (abs(val) > 99999) {
		OLED_print(SymbolSpace); // out of bounds
		return;
	}
	if (val >= 0) {
		OLED_print(SymbolSpace);
		OLED_printNumber(val, 5, true);
	} else {
		OLED_print(SymbolMinus);
		OLED_printNumber(-val, 5, true);
	}
}

void OLED_drawSymbol(uint8_t symbolID) {
	// draw a symbol to the current cursor location
	OLED_setFont(2);
	OLED_drawChar(symbolID + 2);
	OLED_setFont(0);
}

// Draw an area, but y must be aligned on 0/8 offset
void OLED_drawArea(int16_t x, int8_t y, uint8_t wide, uint8_t height,
		const uint8_t *ptr) {
	// Split this from x->x+wide in two strides
	if (x <= -wide)
		return;  // cutoffleft
	if (x > OLED_WIDTH)
		return;      // cutoff right

	uint8_t visibleStart = 0;
	uint8_t visibleEnd = wide;

	// trimming to draw partials
	if (x < 0) {
		visibleStart -= x;  // subtract negative value == add absolute value
	}
	if (x + wide > OLED_WIDTH) {
		visibleEnd = OLED_WIDTH - x;
	}

	for(uint8_t i = 0; i < height/8; i++){
		for (uint8_t xx = visibleStart; xx < visibleEnd; xx++) {
			OLED.firstStripPtr[xx + x + ( (y / 8) * OLED_WIDTH) + ( i * OLED_WIDTH)] = ptr[xx + (i * wide)];
		}
	}

	/*if (y == 0) {
		// Split first line of data
		for (uint8_t xx = visibleStart; xx < visibleEnd; xx++) {
			OLED.firstStripPtr[xx + x] = ptr[xx];
		}
	}

	if (y == 8 || height == 16) {
		// Split the second line
		if(y == 8 && height == 16){
			for (uint8_t xx = visibleStart; xx < visibleEnd; xx++) {
				OLED.secondStripPtr[x + xx] = ptr[xx];
			}
		}else{
			for (uint8_t xx = visibleStart; xx < visibleEnd; xx++) {
				OLED.secondStripPtr[x + xx] = ptr[xx + (height == 16 ? wide : 0)];
			}
		}
	}

	if (y == 16 || height == 24 || (y == 8 && height == 16)) {
		if(y == 16 && height == 16){
			for (uint8_t xx = visibleStart; xx < visibleEnd; xx++) {
				OLED.thirdStripPtr[x + xx] = ptr[xx];
			}
		}else{
			for (uint8_t xx = visibleStart; xx < visibleEnd; xx++) {
				OLED.thirdStripPtr[x + xx] = ptr[xx + (height == 24 ? wide : 0)];
			}
		}
	}
	if (y == 24 || height == 32 || (y == 16 && height == 16)) {
		// Split the second line
		for (uint8_t xx = visibleStart; xx < visibleEnd; xx++) {
			//OLED.secondStripPtr[x + xx] = ptr[xx + (height == 16 ? wide : 0)];
			//OLED.thirdStripPtr[x + xx] = ptr[xx + (height == 32 ? wide : 0)];
			OLED.fourthStripPtr[x + xx] = ptr[xx + (height == 32 ? wide : 0)];
		}
	}*/
}

// Draw an area, but y must be aligned on 0/8 offset
// For data which has octets swapped in a 16-bit word.
void OLED_drawAreaSwapped(int16_t x, int8_t y, uint8_t wide, uint8_t height,
		const uint8_t *ptr) {
	// Split this from x->x+wide in two strides
	if (x <= -wide)
		return;  // cutoffleft
	if (x > OLED_WIDTH)
		return;      // cutoff right

	uint8_t visibleStart = 0;
	uint8_t visibleEnd = wide;

	// trimming to draw partials
	if (x < 0) {
		visibleStart -= x;  // subtract negative value == add absolute value
	}
	if (x + wide > OLED_WIDTH) {
		visibleEnd = OLED_WIDTH - x;
	}

	if (y == 0) {
		// Split first line of data
		for (uint8_t xx = visibleStart; xx < visibleEnd; xx += 2) {
			OLED.firstStripPtr[xx + x] = ptr[xx + 1];
			OLED.firstStripPtr[xx + x + 1] = ptr[xx];
		}
	}
	if (y == 8 || height == 16) {
		// Split the second line
		for (uint8_t xx = visibleStart; xx < visibleEnd; xx += 2) {
			OLED.secondStripPtr[x + xx] = ptr[xx + 1 + (height == 16 ? wide : 0)];
			OLED.secondStripPtr[x + xx + 1] = ptr[xx + (height == 16 ? wide : 0)];
		}
	}
}

void OLED_fillArea(int16_t x, int8_t y, uint8_t wide, uint8_t height,
		const uint8_t value) {
	// Split this from x->x+wide in two strides
	if (x <= -wide)
		return;  // cutoffleft
	if (x > OLED_WIDTH)
		return;      // cutoff right

	uint8_t visibleStart = 0;
	uint8_t visibleEnd = wide;

	// trimming to draw partials
	if (x < 0) {
		visibleStart -= x;  // subtract negative value == add absolute value
	}
	if (x + wide > OLED_WIDTH) {
		visibleEnd = OLED_WIDTH - x;
	}

	if (y == 0) {
		// Split first line of data
		for (uint8_t xx = visibleStart; xx < visibleEnd; xx++) {
			OLED.firstStripPtr[xx + x] = value;
		}
	}
	if (y == 8 || height == 16) {
		// Split the second line
		for (uint8_t xx = visibleStart; xx < visibleEnd; xx++) {
			OLED.secondStripPtr[x + xx] = value;
		}
	}
}

void OLED_drawFilledRect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,
		bool clear) {
	// Draw this in 3 sections
	// This is basically a N wide version of vertical line

	// Step 1 : Draw in the top few pixels that are not /8 aligned
	// LSB is at the top of the screen
	uint8_t mask = 0xFF;
	if (y0) {
		mask = mask << (y0 % 8);
		for (uint8_t col = x0; col < x1; col++)
			if (clear)
				OLED.firstStripPtr[(y0 / 8) * OLED_WIDTH + col] &= ~mask;
			else
				OLED.firstStripPtr[(y0 / 8) * OLED_WIDTH + col] |= mask;
	}
	// Next loop down the line the total number of solids
	if (y0 / 8 != y1 / 8)
		for (uint8_t col = x0; col < x1; col++)
			for (uint8_t r = (y0 / 8); r < (y1 / 8); r++) {
				// This gives us the row index r
				if (clear)
					OLED.firstStripPtr[(r * OLED_WIDTH) + col] = 0;
				else
					OLED.firstStripPtr[(r * OLED_WIDTH) + col] = 0xFF;
			}

	// Finally draw the tail
	mask = ~(mask << (y1 % 8));
	for (uint8_t col = x0; col < x1; col++)
		if (clear)
			OLED.firstStripPtr[(y1 / 8) * OLED_WIDTH + col] &= ~mask;
		else
			OLED.firstStripPtr[(y1 / 8) * OLED_WIDTH + col] |= mask;
}

void OLED_drawHeatSymbol(uint8_t state) {
	// Draw symbol 14
	// Then draw over it, the bottom 5 pixels always stay. 8 pixels above that are
	// the levels masks the symbol nicely
	state /= 31;  // 0-> 8 range
	// Then we want to draw down (16-(5+state)
	uint8_t cursor_x_temp = OLED.cursor_x;
	OLED_drawSymbol(14);
	OLED_drawFilledRect(cursor_x_temp, 0, cursor_x_temp + 12, 2 + (8 - state), true);
}
