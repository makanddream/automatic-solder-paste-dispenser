/*
 * DRV8876.h
 *
 *  Created on: Jan 6, 2021
 *      Author: Alan
 */

#ifndef INC_DRV8876_H_
#define INC_DRV8876_H_

typedef struct drv8876_t{
	uint8_t currentLimitProtection; // Value expressed in mA

	uint8_t currentEndStopDetection; // Value expressed in mA

	uint16_t currentIPROPI; // This value is expressed in mA

	bool isPush; // TODO

	/*
	 * Fault indicator output. Pulled low during a fault condition.
	 *
	 * Faults description (drv8876.pdf | P.16 | Table 7):
	 * 		- ITRIP Indicator					, condition : CBC Mode & IOUT > ITRIP
	 * 		- VM Undervoltage Lockout (UVLO)	, condition : VM < VUVLO
	 * 		- VCP Undervoltage Lockout (CPUV)	, condition : VCP < VCPUV
	 * 		- Overcurrent (OCP)					, condition : IOUT > IOCP
	 * 		- Thermal Shutdown (TSD)			, condition : TJ > TTSD
	 */
	bool fault;
}DRV8876;

void drv8876_driver_start(void);

void drv8876_driver_stop(void);

void drv8876_driver_wakeup(void);

void drv8876_driver_sleep(void);

void drv8876_driver_control(uint8_t speed, bool isClockwise);

void drv8876_speed_control(uint8_t speed);

void drv8876_direction_control(bool isClockwise);

#endif /* INC_DRV8876_H_ */
