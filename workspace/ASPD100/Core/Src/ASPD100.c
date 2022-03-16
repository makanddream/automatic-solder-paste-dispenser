/*
 * ASPD100.c
 *
 *  Created on: Mar 2, 2022
 *      Author: Alan
 */

#include "ASPD100.h"

#include "Translation.h"
#include "configuration.h"

void aspd100_init(aspd100_st* aspd100_st)
{
	uint16_t forwardSteps[12] = {1000, PUSH_TIME_0201, PUSH_TIME_0402, PUSH_TIME_0603, PUSH_TIME_0805, 0, 0, 0, 0, 0, 0, 0};
	uint16_t backwardSteps[12] = {10, RETRACT_TIME_0201, RETRACT_TIME_0402, RETRACT_TIME_0603, RETRACT_TIME_0805, 0, 0, 0, 0, 0, 0, 0};

	uint8_t footprintsCodeTabSize = sizeof(ImperialCode) / sizeof(ImperialCode[0]);

	for(uint8_t i = 0; i < footprintsCodeTabSize; i++){
#ifdef IMPERIAL
		aspd100_st->footPrints[i].footprintName = ImperialCode[i];
#endif

#ifdef METRIC
		aspd100_st->footPrints[i].footprintName = MetricCode[i];
#endif

		aspd100_st->footPrints[i].forwardSteps = forwardSteps[i];
		aspd100_st->footPrints[i].backwardSteps = backwardSteps[i];
	}

	aspd100_st->indexFootPrint = 3;
}
