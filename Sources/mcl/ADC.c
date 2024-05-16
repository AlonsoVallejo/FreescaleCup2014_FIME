/*
 * ADC.c
 *
 *  Created on: May 15, 2014
 *      Created by: Alonso Vallejo
 *      			 Ulises Larios
 *      			 Fernando Alvarez
 */


#include "ADC.h"
#include "derivative.h"


/**************************************************************************************
* 					INIT_ADC0_CAMERA
*					Function to configure the ADC0 for init the 
*					line scan camera analog output.
*					Registers configurations:
*					- ADC_CFG1_MODE( ), adc bits resolution (Eight,Ten, Twelve,Sixteen) 
*					
**************************************************************************************/

void INIT_ADC0_CAMERA(void) // init the ADC0
{
	SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;	// enable the ADC0 port.
	ADC0_CFG1 |= ADC_CFG1_MODE( Ten ) | ADC_CFG1_ADIV(0); // ADC set of bits to a certain value and dividing the frequency conversion
	ADC0_CFG2 |= ADC_CFG2_MUXSEL_MASK; // select channel B ADxxB
	ADC0_CFG2 |= ADC_CFG2_ADHSC_MASK; // High speed conversion.
	ADC0_CFG2 |= ADC_CFG2_ADLSTS_MASK; // 2 extra ASDCK cycles
	ADC0_SC1A = ADC_SC1_ADCH(31);
	
}

/*********************************************************************
* 				ADC0_READ_CAMERA
*				Function to read the A0 port of the line scan camera
*					Parameters:
*					PORT	select the FRDM pin out to read the ADC
*					
*					return:
*						the result value of the adc conversion.
*********************************************************************/

unsigned int ADC0_READ_CAMERA(unsigned short PORT) // function for read the ADC.
{
	
	ADC0_SC1A = ADC_SC1_ADCH(PORT); // select the  reading port.
	while(ADC0_SC2 & ADC_SC2_ADACT_MASK);  	// while the conversion is process.	
	while(!(ADC0_SC1A & ADC_SC1_COCO_MASK));// when the conversion ends.		
	return ADC0_RA; // return the conversion value.
}


