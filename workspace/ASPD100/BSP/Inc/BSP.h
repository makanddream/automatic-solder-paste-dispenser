#include "BSP_Flash.h"
#include "stdint.h"
/*
 * BSP.h -- Board Support
 *
 * This exposes functions that are expected to be implemented to add support for different hardware
 */

#ifndef BSP_BSP_H_
#define BSP_BSP_H_
#ifdef __cplusplus
extern "C" {
#endif

// Called to reset the hardware watchdog unit
void resetWatchdog();

// Readers for the buttons
// !! Returns 1 if held down, 0 if released
uint8_t getButtonRight();
uint8_t getButtonLeft();
uint8_t getButtonUp();
uint8_t getButtonDown();
uint8_t getButtonCenter();
uint8_t getButtonAction();

// This is a work around that will be called if I2C starts to bug out
// This should toggle the SCL line until SDA goes high to end the current transaction
void unstick_I2C();

// Reboot the IC when things go seriously wrong
void reboot();

// If the user has programmed in a bootup logo, draw it to the screen from flash
// Returns 1 if the logo was printed so that the unit waits for the timeout or button
uint8_t showBootLogoIfavailable();
//delay wrapper for delay using the hardware timer (used before RTOS)
void delay_ms(uint16_t count) ;
//Used to allow knowledge of if usb_pd is being used
uint8_t usb_pd_detect();

#ifdef __cplusplus
}
#endif
#endif /* BSP_BSP_H_ */
