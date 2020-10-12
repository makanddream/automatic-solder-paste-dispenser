/**
 * This Library was originally written by Olivier Van den Eede (4ilo) in 2016.
 * Some refactoring was done and SPI support was added by Aleksander Alekseev (afiskon) in 2018.
 *
 * https://github.com/afiskon/stm32-ssd1306
 */

#ifndef __OLED_SSD1306_H__
#define __OLED_SSD1306_H__

#include <stddef.h>
#include <_ansi.h>

_BEGIN_STD_C

#include <BSP.h>
#include <stdbool.h>
#include <string.h>
#include "I2C_Wrapper.h"
#include "Font.h"

/* vvv I2C config vvv */

#ifndef OLED_SSD1306_I2C_ADDR
#define OLED_SSD1306_I2C_ADDR        (0x3C << 1)
#endif

// SSD1306 OLED height in pixels
#ifndef OLED_SSD1306_HEIGHT
#define OLED_SSD1306_HEIGHT          64
#endif

// SSD1306 width in pixels
#ifndef OLED_SSD1306_WIDTH
#define OLED_SSD1306_WIDTH           128
#endif

// some LEDs don't display anything in first two columns
// #define OLED_SSD1306_WIDTH           130

#ifndef OLED_SSD1306_BUFFER_SIZE
#define OLED_SSD1306_BUFFER_SIZE   OLED_SSD1306_WIDTH * OLED_SSD1306_HEIGHT / 8
#endif

// Enumeration for screen colors
typedef enum {
    Black = 0x00, // Black color, no pixel
    White = 0x01  // Pixel is set. Color depends on OLED
} OLED_SSD1306_COLOR;

typedef enum {
	OLED_SSD1306_OK = 0x00,
	OLED_SSD1306_ERR = 0x01  // Generic error.
} OLED_SSD1306_Error_t;

// Struct to store transformations
typedef struct {
    uint16_t CurrentX;
    uint16_t CurrentY;
    uint8_t Inverted;
    uint8_t Initialized;
    uint8_t DisplayOn;
} OLED_SSD1306_t;

typedef struct {
    uint8_t x;
    uint8_t y;
} OLED_SSD1306_VERTEX;

typedef struct {
	uint8_t FontWidth;    	/*!< Font width in pixels */
	uint8_t FontHeight;   			/*!< Font height in pixels */
	const uint8_t *currentFont; 	/*!< Pointer to data font data array */
} OLED_SSD1306_FontDef;

// Procedure definitions
void oled_ssd1306_Init(void);
void oled_ssd1306_Fill(OLED_SSD1306_COLOR color);
void oled_ssd1306_UpdateScreen(void);
void oled_ssd1306_DrawPixel(uint8_t x, uint8_t y, OLED_SSD1306_COLOR color);
char oled_ssd1306_WriteChar(char ch, OLED_SSD1306_FontDef Font, OLED_SSD1306_COLOR color);
char oled_ssd1306_WriteString(char* str, OLED_SSD1306_FontDef Font, OLED_SSD1306_COLOR color);
void oled_ssd1306_SetCursor(uint8_t x, uint8_t y);
void oled_ssd1306_Line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, OLED_SSD1306_COLOR color);
void oled_ssd1306_DrawArc(uint8_t x, uint8_t y, uint8_t radius, uint16_t start_angle, uint16_t sweep, OLED_SSD1306_COLOR color);
void oled_ssd1306_DrawCircle(uint8_t par_x, uint8_t par_y, uint8_t par_r, OLED_SSD1306_COLOR color);
void oled_ssd1306_Polyline(const OLED_SSD1306_VERTEX *par_vertex, uint16_t par_size, OLED_SSD1306_COLOR color);
void oled_ssd1306_DrawRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, OLED_SSD1306_COLOR color);

void oled_ssd1306_setFont(uint8_t fontNumber);
uint8_t oled_ssd1306_getFont(void);
/**
 * @brief Sets the contrast of the display.
 * @param[in] value contrast to set.
 * @note Contrast increases as the value increases.
 * @note RESET = 7Fh.
 */
void oled_ssd1306_SetContrast(const uint8_t value);
/**
 * @brief Set Display ON/OFF.
 * @param[in] on 0 for OFF, any for ON.
 */
void oled_ssd1306_SetDisplayOn(const uint8_t on);
/**
 * @brief Reads DisplayOn state.
 * @return  0: OFF.
 *          1: ON.
 */
uint8_t oled_ssd1306_GetDisplayOn();

// Low-level procedures
void oled_ssd1306_Reset(void);
void oled_ssd1306_WriteCommand(uint8_t byte);
void oled_ssd1306_WriteData(uint8_t* buffer, size_t buff_size);
OLED_SSD1306_Error_t oled_ssd1306_FillBuffer(uint8_t* buf, uint32_t len);

_END_STD_C

#endif // __OLED_SSD1306_H__
