/*
 * Speedsensors.c
 *
 *  Created on: Feb 16, 2015
 *      Author: Vallejo
 */

#include "Speedsensors.h"

/********************************************************************************
						Definitions used in the  process
********************************************************************************/ 
//10 samples
#define ac_speed_sensor_right	(11.9) //5.4 
#define ac_speed_sensor_left	(11.75)
/********************************************************************************
						Variables used in the  process
********************************************************************************/ 
unsigned int  right_tire_pulses, 
			  left_tire_pulses;
			 
unsigned int  state_sps_right,
			  state_sps_left,
			  risingflag_righ,
			  risingflag_left;

volatile unsigned int right_tire_value,
					  left_tire_value; 

/********************************************************************************
						Function to read the number of pulses detected
						by the speed sensors.
********************************************************************************/ 

void SPEED_SENSORS_TASK(void)
{
	state_sps_right = STATE_SPEEDSENSOR_1();
	state_sps_left = STATE_SPEEDSENSOR_0();
	
	if(sampling_speed <= samples_number_ss)
	{
		switch(state_sps_right)
		{
		case 1: 
			risingflag_righ = 1; 
			break;
		case 0: 
			if(risingflag_righ == 1)
			{ 
				risingflag_righ = 0;
				right_tire_pulses = right_tire_pulses + 1;
			}
		}

	switch(state_sps_left)
		{
		case 1: 
			risingflag_left = 1; 
			break;
		case 0: 
			if(risingflag_left == 1)
			{ 
				risingflag_left = 0;
				left_tire_pulses = left_tire_pulses + 1;
			}
		}
	}
	SPEED_SENSORS_CONTROL();
}

/********************************************************************************
						Function to convert the speed sensors pulses
						in values equivalent of PWM generated.
********************************************************************************/ 

void SPEED_SENSORS_CONTROL(void)
{
	if(sampling_speed >= samples_number_ss)
	{
		right_tire_value = right_tire_pulses * ac_speed_sensor_right;
		left_tire_value = left_tire_pulses * ac_speed_sensor_left;
		
		if(right_tire_value > 255) right_tire_value = 255;
		if(left_tire_value > 255) left_tire_value = 255;
	}
}
