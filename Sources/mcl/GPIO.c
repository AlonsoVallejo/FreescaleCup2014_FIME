/*
 * GPIO.c
 *
 *  Created on: Aug 15, 2014
 *      Author: Alonso Vallejo
 *      		Ulises Larios
 *      		Fernando Alvarez
 */


#include "GPIO.h"

/*********************************************************************
* 					Init_Camera
*					Function to init the line scan camera with the
*					FRDM ports.
*					Parameters: 
*						--
*					
*					return:	
*						--
*********************************************************************/

void Init_Camera (void)
{
	SIM_SCGC5 |=  SIM_SCGC5_PORTE_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTA_MASK;  // enable ports E and D.
	
	PORTD_PCR7   = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK; //configure the PTD3 is GPIO.
	PORTE_PCR1   = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK; //configure the PTC1 is GPIO.
	
	PORTD_PCR5 = PORT_PCR_MUX(0); //Make sure AO signal goes to an analog input
	PORTD_PCR6 = PORT_PCR_MUX(0); //Make sure AO signal goes to an analog input

	GPIOD_PDDR |= (1 << 7); // Make sure the CLK and SI pins are outputs
	GPIOE_PDDR |= (1 << 1); // TAOS clock pin
	
	CAMERA_CLK_LOW;
	CAMERA_SI_LOW;
	
	PORTA_PCR12   = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK; //configure the PTD3 is GPIO.
	GPIOA_PDDR |= (1 << 12); // Make sure the CLK and SI pins are outputs
	
	PORTA_PCR13   = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK; //configure the PTD3 is GPIO.
	GPIOA_PDDR |= (1 << 13); // Make sure the CLK and SI pins are outputs

	
}

/*********************************************************************
* 					INIT_GPIO_MOTORS
*					Function to init the gpio ports uses to send
*					the signals to control the TFC shield.
*					FRDM ports.
*					Parameters:
*						--
*					
*					return:		
*						--
*********************************************************************/

void INIT_GPIO_MOTORS (void)

{
	SIM_SCGC5 |=  SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTE_MASK; // Enable wishes ports.
	
	PORTE_PCR21  = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK; //configure the PTE21 as GPIO (ENABLE)
	
	GPIOE_PDDR |= (1 << 21); // Enable H - bridge pin
	ENABLE_H_BRIGDE;
}

/*********************************************************************
* 					INIT_SPEED_SENSORS
*					Function to init the gpio ports uses to read
*					the speed sensors.
*					
*					Parameters:
*						--
*					
*					return:		
*						--
*********************************************************************/

void INIT_SPEED_SENSORS (void)

{
	SIM_SCGC5 |=  SIM_SCGC5_PORTA_MASK; // Enable wishes ports.
	
	PORTA_PCR1  = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK; //configure the PTA1 as GPIO (ENABLE)
	PORTA_PCR2  = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK; //configure the PTA2 as GPIO (ENABLE)
}

//*********************************************************************
//			FUNCTION FOR READ THE STATE OF LEFT SPEED SENSOR(PTA1)
//*********************************************************************

unsigned int STATE_SPEEDSENSOR_0(void) 
{
	unsigned short SPS1_Val=0;
	SPS1_Val = (GPIOA_PDIR>>1) & 0x1;
	return SPS1_Val;		
}

//*********************************************************************
//			FUNCTION FOR READ THE STATE OF RIGHT SPEED SENSOR(PTA2)
//*********************************************************************

unsigned int STATE_SPEEDSENSOR_1(void) 
{
	unsigned short SPS2_Val=0;
	SPS2_Val = (GPIOA_PDIR>>2) & 0x1;
	return SPS2_Val;		
}

