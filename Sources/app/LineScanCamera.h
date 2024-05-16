/*
 * LineScanCamera.h
 *
 *  Created on: May 22, 2014
 *      Created by: Alonso Vallejo
 *      			Ulises Larios
 *      			Fernando Alvarez
 */

#ifndef LINESCANCAMERA_H_
#define LINESCANCAMERA_H_

/********************************************************************************
						libraries used.
********************************************************************************/ 
#include "SERVO.h"
#include "..\mcl\ADC.h"
#include "..\mcl\GPIO.h"
#include "..\sdl\KL25Z_SHIELD_INTERFACE.h"
#include "Speedsensors.h"
/********************************************************************************
						Functions used in APP..
********************************************************************************/ 
void TAOS_CLK_SI_SIGNALS (void);
void TAOS_CAPTURE_DATA (void);
void TAOS_IMAGE_PROCESS(void);
void edge_first_smallest(void);
void search_finish_line(void);
void search_finish_line_L(void);
void search_finish_line_R(void);
void finish_line_detected (void);


/********************************************************************************
						Variables used in other functions.
********************************************************************************/ 
#define samples_number_ss		(10)

volatile float  error;
volatile unsigned int count_pixel;
volatile unsigned int zeros_number;
volatile unsigned int final;
volatile unsigned int micontador,sampling_speed;
volatile unsigned int finish_flag;
#endif /* LINESCANCAMERA_H_ */
