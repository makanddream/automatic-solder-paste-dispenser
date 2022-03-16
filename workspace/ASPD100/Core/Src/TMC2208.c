/*
 * TMC2208.c
 *
 *  Created on: 2 mars 2022
 *      Author: Alan
 */

#include "TMC2208.h"

#include "main.h"

void tmc2208_init(tmc2208_st* tmc2208_st)
{
	//Initialization
	tmc2208_change_direction(tmc2208_st, E_FORWARD);
}

void tmc2208_change_direction(tmc2208_st* tmc2208_st, direction_et direction)
{
	tmc2208_st->direction = direction;
	if(tmc2208_st->direction == E_FORWARD){
		HAL_GPIO_WritePin(DIR_GPIO_Port, DIR_Pin, GPIO_PIN_RESET);
	}else{
		HAL_GPIO_WritePin(DIR_GPIO_Port, DIR_Pin, GPIO_PIN_SET);
	}
}

void tmc2208_toggle_direction(tmc2208_st* tmc2208_st)
{
	if(tmc2208_st->direction == E_FORWARD){
		tmc2208_st->direction = E_BACKWARD;
		HAL_GPIO_WritePin(DIR_GPIO_Port, DIR_Pin, GPIO_PIN_SET);
	}else{
		tmc2208_st->direction = E_FORWARD;
		HAL_GPIO_WritePin(DIR_GPIO_Port, DIR_Pin, GPIO_PIN_RESET);
	}
}
