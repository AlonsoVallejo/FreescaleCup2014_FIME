#ifndef ADC_H_
#define ADC_H_

/********************************************************************************
						libraries used.
********************************************************************************/ 
#include "derivative.h"
//**************************************************
//					PINS FOR ADC INPUTS
//**************************************************

// definitions of the port that it can used for the ADC reading.

#define PTB0 (8)
#define PTB1 (9)
#define PTB2 (12)
#define PTB3 (13)
#define PTC2 (11)
#define PTC1 (15)
#define PTD5 (6)
#define PTD6 (7)

// definitions for the configuration of the ADC resolution.

#define Eight  	(0)
#define Ten    	(2)
#define Twelve 	(1)
#define Sixteen (3)

/********************************************************************************
						Functions used.
********************************************************************************/ 
void INIT_ADC0_CAMERA(void);
unsigned int ADC0_READ_CAMERA(unsigned short PORT); 

#endif /* ADC_H_ */
