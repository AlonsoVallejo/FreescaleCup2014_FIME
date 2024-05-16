/*
 * Speedsensors.h
 *
 *  Created on: Feb 16, 2015
 *      Author: Vallejo
 */

#ifndef SPEEDSENSORS_H_
#define SPEEDSENSORS_H_

/********************************************************************************
						libraries used.
********************************************************************************/ 
#include "derivative.h"
#include "..\mcl\GPIO.h"
#include "LineScanCamera.h"

/********************************************************************************
						Functions used in APP..
********************************************************************************/ 
void SPEED_SENSORS_TASK(void);
void SPEED_SENSORS_CONTROL(void);
/********************************************************************************
						Variables used in other functions.
********************************************************************************/ 
unsigned int right_tire_pulses, left_tire_pulses;
volatile unsigned int sampling_speed,
					  right_tire_value,
					  left_tire_value; 

#endif /* SPEEDSENSORS_H_ */
