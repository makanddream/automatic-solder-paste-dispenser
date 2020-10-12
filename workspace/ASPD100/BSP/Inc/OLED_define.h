/*
 * OLED_define.h
 *
 *  Created on: Sept 28, 2020
 *      Author: Alan
 *  Information extracted form the SSD1306_v1.1.pdf datasheet
 */

#ifndef OLED_DEFINE_H_
#define OLED_DEFINE_H_

/*
 * Read mode for I2C :
 * 	I2C address = 0x3D
 *
 * Write mode for I2C :
 * 	I2C address = 0x3C
 * 	Control byte = Co | D/C | b5 | b4 | b3 | b2 | b1 | b0
 * 		- If the Co bit is set as logic “0”, the transmission of the following information will contain
 * 		  data bytes only.
 * 		- The D/C# bit determines the next data byte is acted as a command or a data.
 * 				- D/C# = “0” : it defines the following data byte as a command.
 * 				- D/C# = “1” : it defines the following data byte as a data which will be stored at the GDDRAM.
 */

#define CMD_TYPE		0x80 //0b1000 0000
#define DATA_TYPE		0x40 //0b0100 0000

/*#define MULTIPLE_CMD_TYPE	0x80 //0b1000 0000
#define ONE_CMD_TYPE		0x00 //0b0000 0000

#define MULTIPLE_DATA_TYPE		0xC0 //0b1100 0000
#define ONE_DATA_TYPE			0x40 //0b0100 0000*/

#define CMD_SET_MEMORY_ADDRESSING_MODE	0x20 //0b0010 0000
#define CMD_SET_COLUMN_ADDRESS			0x21 //0b0010 0001
#define CMD_SET_PAGE_ADDRESS			0x22 //0b0010 0010

/* Entire Display ON (A4h/A5h)
 *
 */
#define CMD_FORCE_ENTIRE_DISPLAY_ON		0xA5
#define CMD_RESUME_DISPLAY_TO_GDDRAM	0xA4

/* Set Display ON/OFF (AEh/AFh)
 *
 */
#define CMD_DISPLAY_ON					0xAF
#define CMD_DISPLAY_OFF					0xAE

/* Set Normal/Inverse Display (A6h/A7h)
 *
 */
#define CMD_SET_NORMAL_DISPLAY		0xA6
#define CMD_SET_INVERSE_DISPLAY		0xA7

/* Set Contrast Control for BANK0 (81h)
 *
 */
#define CMD_SET_CONTRAST_CONTROL	0x81

/* Set Display Offset (D3h)
 *
 */
#define CMD_SET_DISPLAY_OFFSET		0xD3

/* Set COM Pins Hardware Configuration (DAh)
 *
 */
#define CMD_SET_COM_PINS_HW_CONFIG		0xDA

/* Set VCOMH Deselect Level (DBh)
 * 		This command adjusts the VCOMH regulator output.
 */
#define CMD_SET_V_COMH_DESELECT_LEVEL		0xDB


// Set Display Start Line (40h~7Fh)
#define CMD_SET_DISPLAY_START_LINE_0	0x40 //RAM row 0 is mapped to COM0
#define CMD_SET_DISPLAY_START_LINE_1	0x41 //RAM row 1 is mapped to COM0
// .....

/* Only for page addressing mode (0x20 0x10) */

// Set Lower Column Start Address for Page Addressing Mode (00h~0Fh)
#define CMD_SET_LOWER_COLUMN_0_START_ADDRESS	0x00
#define CMD_SET_LOWER_COLUMN_1_START_ADDRESS	0x01
#define CMD_SET_LOWER_COLUMN_2_START_ADDRESS	0x02
#define CMD_SET_LOWER_COLUMN_3_START_ADDRESS	0x03
#define CMD_SET_LOWER_COLUMN_4_START_ADDRESS	0x04
#define CMD_SET_LOWER_COLUMN_5_START_ADDRESS	0x05
#define CMD_SET_LOWER_COLUMN_6_START_ADDRESS	0x06
#define CMD_SET_LOWER_COLUMN_7_START_ADDRESS	0x07
#define CMD_SET_LOWER_COLUMN_8_START_ADDRESS	0x08
#define CMD_SET_LOWER_COLUMN_9_START_ADDRESS	0x09
#define CMD_SET_LOWER_COLUMN_10_START_ADDRESS	0x0A
#define CMD_SET_LOWER_COLUMN_11_START_ADDRESS	0x0B
#define CMD_SET_LOWER_COLUMN_12_START_ADDRESS	0x0C
#define CMD_SET_LOWER_COLUMN_13_START_ADDRESS	0x0D
#define CMD_SET_LOWER_COLUMN_14_START_ADDRESS	0x0E
#define CMD_SET_LOWER_COLUMN_15_START_ADDRESS	0x0F

// Set Higher Column Start Address for Page Addressing Mode (10h~1Fh)


// Set Page Start Address for Page Addressing Mode (B0h~B7h)
#define CMD_SET_PAGE_0_START_ADDRESS	0xB0
#define CMD_SET_PAGE_1_START_ADDRESS	0xB1
#define CMD_SET_PAGE_2_START_ADDRESS	0xB2
#define CMD_SET_PAGE_3_START_ADDRESS	0xB3
#define CMD_SET_PAGE_4_START_ADDRESS	0xB4
#define CMD_SET_PAGE_5_START_ADDRESS	0xB5
#define CMD_SET_PAGE_6_START_ADDRESS	0xB6
#define CMD_SET_PAGE_7_START_ADDRESS	0xB7


#define DEVICEADDR_OLED   (0x3c << 1)
#define OLED_WIDTH        128
#define OLED_HEIGHT       32
//#define FRAMEBUFFER_START 19

#endif /* OLED_DEFINE_H_ */
