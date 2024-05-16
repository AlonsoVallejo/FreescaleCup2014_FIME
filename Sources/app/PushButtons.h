/*
 * PushButtons.h
 *
 *  Created on: Mar 7, 2015
 *      Author: Vallejo
 */

#ifndef PUSHBUTTONS_H_
#define PUSHBUTTONS_H_


/********************************************************************************
						libraries used.
********************************************************************************/ 
#include "derivative.h"
#include "..\sdl\KL25Z_SHIELD_INTERFACE.h"

/********************************************************************************
						Functions used in APP..
********************************************************************************/ 
void PUSH_BUTTON_TASK(void);
/********************************************************************************
						Variables used in other functions.
********************************************************************************/ 
unsigned int state_push_button_A,
			 buttonA_pressed,
			 count_buttonA;

#endif /* PUSHBUTTONS_H_ */
