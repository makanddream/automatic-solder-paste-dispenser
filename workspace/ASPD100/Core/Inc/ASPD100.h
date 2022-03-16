/*
 * ASPD100.h
 *
 *  Created on: Mar 2, 2022
 *      Author: Alan
 */

#ifndef INC_ASPD100_H_
#define INC_ASPD100_H_

#include <stdio.h>
#include <stdbool.h>

typedef struct{
	const char* footprintName;

	/*
	 * Number of steps forward, to push the syringe
	 */
	uint16_t forwardSteps;

	/*
	 * Number of steps backward, to retract the syringe
	 */
	uint16_t backwardSteps;
} footprint_st;

typedef struct{
	footprint_st footPrints[12];
	uint8_t indexFootPrint;
}aspd100_st;

void aspd100_init(aspd100_st* aspd100_st);

#endif /* INC_ASPD100_H_ */
