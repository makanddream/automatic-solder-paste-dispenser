/*
 * TMC2208.h
 *
 *  Created on: 2 mars 2022
 *      Author: Alan
 */

#ifndef INC_TMC2208_H_
#define INC_TMC2208_H_

typedef enum{
    E_FORWARD,
    E_BACKWARD,
}direction_et;

typedef struct{

	direction_et direction;
}tmc2208_st;

void tmc2208_init(tmc2208_st* tmc2208_st);

void tmc2208_change_direction(tmc2208_st* tmc2208_st, direction_et direction);

void tmc2208_toggle_direction(tmc2208_st* tmc2208_st);

#endif /* INC_TMC2208_H_ */
