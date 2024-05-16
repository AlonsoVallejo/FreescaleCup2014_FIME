/*
 * PushButtons.c
 *
 *  Created on: Mar 7, 2015
 *      Author: Vallejo
 */

#include "PushButtons.h"

/********************************************************************************
						Variables used in the  process
********************************************************************************/ 
unsigned int state_push_button_A,
			 buttonA_pressed,
			 count_buttonA;

/********************************************************************************
						Function to read the number of pulses detected
						of the push button.
********************************************************************************/ 
void PUSH_BUTTON_TASK(void)
{
	state_push_button_A = PUSH_BUTTON_1_STATE();
	
		switch(state_push_button_A)
		{
		case 1: 
			buttonA_pressed = 1; 
			break;
		case 0: 
			if(buttonA_pressed == 1)
			{ 
				buttonA_pressed = 0;
				count_buttonA = count_buttonA + 1;
				if(count_buttonA == 2) count_buttonA = 0;
			}
		}
}
