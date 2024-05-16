/*
 * DC_Motors_Control.h
 *
 *  Created on: Aug 20, 2014
 *      Author: Alonso Vallejo
 *      		Ulises Larios
 *      		Fernando Alvarez
 */

#ifndef DC_MOTORS_CONTROL_H_
#define DC_MOTORS_CONTROL_H_

/********************************************************************************
						libraries used.
********************************************************************************/ 
#include "LineScanCamera.h"
#include "..\mcl\tpm.h"
#include "..\mcl\GPIO.h"
#include "derivative.h"
#include "..\sdl\KL25Z_SHIELD_INTERFACE.h"
#include "Speedsensors.h"
#include "PushButtons.h"
/********************************************************************************
						Functions used in APP..
********************************************************************************/ 
void DC_MOTORS_CONTROL(void);
void DC_MOTORS_SPEED(void);
void DC_MOTORS(void);
void DC_DP_parameters(void);
void DC_DP_parameters_test(void);
void stop_car(void);
void reset_values(void);

#endif /* DC_MOTORS_CONTROL_H_ */
