/*
 * SERVO.h
 *
 *  Created on: Jun 23, 2014
 *       Created by: Alonso Vallejo
 *      			 Ulises Larios
 *      			 Fernando Alvarez
 */

#ifndef SERVO_H_
#define SERVO_H_

/********************************************************************************
						libraries used.
********************************************************************************/ 
#include "LineScanCamera.h"
#include "..\mcl\tpm.h"
#include "..\sdl\KL25Z_SHIELD_INTERFACE.h"
#include "..\mcl\GPIO.h"


/********************************************************************************
						Definitions used in servo control.
********************************************************************************/
#define FTM1_OVERFLOW_FREQUENCY 50  // Desired Frequency of PWM Signal - Here 50Hz => 20ms period
// use these to dial in servo steering to your particular servo
#define SERVO_MIN_DUTY_CYCLE              432  // The number here should be be *pulse width* in seconds to move servo to its left limit
#define SERVO_MAX_DUTY_CYCLE              692  // The number here should be be *pulse width* in seconds to move servo to its Right limit
#define Angle_Max	(.541)

/********************************************************************************
						Variables used in other functions.
********************************************************************************/ 
float E_ant;

/********************************************************************************
						Functions used in APP.
********************************************************************************/ 
void STEERING_SERVO(float Radians);
void SERVO_DIRECTION_CONTROL(void);

#endif /* SERVO_H_ */
