/*
 * LineScanCamera.c
 *
 *  Created on: May 22, 2014
 *      Created by: Alonso Vallejo
 *      			Ulises Larios
 *      			Fernando Alvarez
 */

#include "LineScanCamera.h"

/********************************************************************************
						Definitions used in the camera process
********************************************************************************/ 

#define taos_pixel_max   		(113) 		// 	number biggest the pixel is consider in the camera`
#define taos_pixel_min	 		(15)  		// 	number smallest the pixel is consider in the camera`
#define setpoint_camera  		(64)  		// 	number of the pixel that is used like reference to obtain the error.
#define Threshold 		 		(15)  		// 
#define range_smallests_values  (100)		// 
#define Calib_const				(0.7) 	    //	0 -> 1
#define max_adc_val				(1025)
#define min_adc_val				(0)

/**************************************************	
 	 	 	 Define the color of the surface.
 0 -> The surface color is different of the white.
 1 -> The surface color is white.
 **************************************************/
#define whitesurface 0

/**************************************************	
 	 	 	 Define the finis line function.
 0 -> function using the camera and POT2. 
 1 -> Function using only the camera.(only if Whitesurface = 0)
 **************************************************/
#define FinishLineSearchMode 0

/********************************************************************************
						Variables used in the camera process
********************************************************************************/ 

volatile unsigned int count_pixel,
					  count_pixel2, 
					  count_zeros = 0,
					  zeros_on_track = 0,
					  first_zero,
					  zeros_number = 0,
					  calibration_to_zeros,
					  Dig_val_ant,
					  Dig_val,
					  count_edges;

static unsigned int taos_read_value[128]; // analog values of the each pixel of the camera
static int taos_digital_value[128]; // analog values of the camera converted in digital values

static unsigned int edge;  // variables used to detected a black line

static unsigned int taos_calibration,  // variables used to calibrated the camera
					smallest_min_val_read,
					min_val_read,
					min_val_read2,
					max_val_read,
					min_val_read_2,
					max_val_read_2,
					taos_adc_high_level;
					
volatile unsigned int final = 0,
					  leds_camera,
					  sampling_speed,
					  count_1_R,
					  count_1_L,
					  count_0_R,
					  count_0_L,
					  count_1_L_B,
					  count_1_R_B,
					  count_switch,
					  width_ones,
					  width_zeros,
					  vision_width,
					  val_ones_zeros,
					  finish_flag = 0,
					  zeros_pot,
					  line_pos_1,
					  line_pos_2,
					  flag,
					  flag2; 

volatile float  error;

/********************************************************************************
						Principal state machine that include all process 
						to generated Taos signals, getting date and image 
						process
********************************************************************************/ 
void TAOS_CLK_SI_SIGNALS (void) //state machine for generate the CLK and SI TAOS signals.
{
	static unsigned short quarter_period_count = 0; // 1/4 of the CLK and SI peroid signals for each case.
	static unsigned short count_CLK_pulse = 0;
switch(quarter_period_count)
	{
	case 0:
		//PTA12_TEST_HIGH;
		taos_adc_high_level = POT1_ADC_VALUE();
		CAMERA_SI_LOW;
		CAMERA_CLK_LOW;
		quarter_period_count = 1;
		sampling_speed++;
		break;
	case 1:
		zeros_pot = POT2_ADC_VALUE()*0.125;
		CAMERA_SI_HIGH;
		CAMERA_CLK_LOW;
		quarter_period_count = 2;
		break;
	case 2:
		CAMERA_SI_HIGH;
		CAMERA_CLK_HIGH;
		quarter_period_count = 3;
		break;
	case 3:
		CAMERA_SI_LOW;
		CAMERA_CLK_HIGH;
		quarter_period_count = 4;
		break;
	case 4:
		CAMERA_SI_LOW;
		CAMERA_CLK_LOW;
		quarter_period_count = 5;
		count_pixel = 0;
		break;
	default:
			if(quarter_period_count <= (256+4))
			{
				switch(count_CLK_pulse)
				{
				case 0:
					CAMERA_CLK_HIGH;
					TAOS_CAPTURE_DATA(); 
					TAOS_IMAGE_PROCESS();
					quarter_period_count++;
					count_CLK_pulse = 1;
					count_pixel =count_pixel + 1;
					if(count_pixel >= 128) count_pixel = 128;
					break;
				case 1:
					CAMERA_CLK_LOW;
					quarter_period_count++;
					count_CLK_pulse = 0;
					SPEED_SENSORS_TASK();
					break;
				}
			}
			else quarter_period_count=0;
	}
}

/********************************************************************************
						Function that is used to auto calibrate the camera
						to detect the way edges and the camera digit.
********************************************************************************/ 

void TAOS_CAPTURE_DATA (void) 
{	
	//PTA12_TEST_HIGH;
	taos_read_value[count_pixel] = ADC0_READ_CAMERA(PTD5);// read the pixels values of the camera and convert to digital values
	//PTA12_TEST_LOW;
	if(count_pixel == taos_pixel_max)		//
	 {
		    max_val_read=min_adc_val;
		 	min_val_read=max_adc_val;
		 	max_val_read_2=min_adc_val;
		 	min_val_read_2=max_adc_val;
		 	for(count_pixel2 = taos_pixel_min; count_pixel2 <=taos_pixel_max; count_pixel2++)
		 	{
		 		if(max_val_read<taos_read_value[count_pixel2]) max_val_read = taos_read_value[count_pixel2];
		 		if(min_val_read>taos_read_value[count_pixel2])
		 		{
				if(whitesurface == 1) edge = count_pixel2; //detects the smallest value without noise delete
		 		min_val_read = taos_read_value[count_pixel2];
		 		}
		 		if((count_pixel2 >= taos_pixel_min + 10)&& (count_pixel2 <=taos_pixel_max-10))
		 		{
		 			if(max_val_read_2<taos_read_value[count_pixel2]) max_val_read_2 = taos_read_value[count_pixel2];
		 			if(min_val_read_2>taos_read_value[count_pixel2])min_val_read_2 = taos_read_value[count_pixel2];
		 		}
		 	}
		 	taos_calibration = ((max_val_read -  min_val_read)*Calib_const) + min_val_read;  //
		 	if(taos_calibration >= taos_adc_high_level)  taos_calibration = taos_adc_high_level; ///
		 	vision_width = max_val_read_2 -  min_val_read_2;
		 	calibration_to_zeros = ((vision_width)*Calib_const) + min_val_read_2;
		 	
	 }
	if(taos_read_value[count_pixel] >= taos_calibration) taos_digital_value[count_pixel] = 1; //
	else taos_digital_value[count_pixel] = -1; 
}


/******************************************************************************
* 					find the first smallest value with noise delete
******************************************************************************/
void edge_first_smallest(void)
{
	if(count_pixel == taos_pixel_max)
	 {
		 min_val_read = max_adc_val;
		 min_val_read2 = max_adc_val;
		 count_1_R = 0;
		 count_1_L = 0;
		 count_0_R = 0;
		 count_0_L = 0;
		 count_1_L_B = 0;
		 count_1_R_B = 0;
		 count_switch = 0;
		 // 	  | ---->
		 
		 for(count_pixel2 = setpoint_camera; count_pixel2 <= taos_pixel_max; count_pixel2++)
		 {
			  if(min_val_read > taos_read_value[count_pixel2])
			  {
				min_val_read = taos_read_value[count_pixel2];
			  }
			  search_finish_line_R(); //| - - | -->
		 }
		 count_switch = 0;
		 //	<---- |
		 for(count_pixel2 = setpoint_camera; count_pixel2 >= taos_pixel_min; count_pixel2--)
		 {
		 	  if(min_val_read2 > taos_read_value[count_pixel2])
		 	  {
		 		min_val_read2 = taos_read_value[count_pixel2];
		 	  }
		 	  search_finish_line_L(); //<-- | - - |
		 }
		 
		 if (min_val_read > min_val_read2) smallest_min_val_read = min_val_read2;
		 if (min_val_read < min_val_read2) smallest_min_val_read = min_val_read;
		 
		 if(E_ant <= -0.25)
		 {
			 for(count_pixel2 = taos_pixel_min; count_pixel2 <= taos_pixel_max; count_pixel2++)
			 {
				 if(taos_read_value[count_pixel2] < smallest_min_val_read + range_smallests_values &&
					taos_read_value[count_pixel2] > smallest_min_val_read - range_smallests_values 	)	
				 {
					 if(count_zeros == 0) {
						 edge = count_pixel2;
						 flag = 1;
					 }
				 	 count_zeros = count_zeros + 1;
				 }  
				 if(min_val_read > taos_read_value[count_pixel2])
				 {
					 min_val_read = taos_read_value[count_pixel2];
				 }
			 }
		 }
		 
		 else if(E_ant >= 0.25)
		 {
			 for(count_pixel2 = taos_pixel_max; count_pixel2 >= taos_pixel_min; count_pixel2--)
			 {
				 if(taos_read_value[count_pixel2] < smallest_min_val_read + range_smallests_values &&
					taos_read_value[count_pixel2] > smallest_min_val_read - range_smallests_values 	)	
				 {
					 if(count_zeros == 0)  {
						 edge = count_pixel2;
						 flag = 2;
					 }
					 count_zeros = count_zeros + 1;
				 } 
			 }
		}
		else
		{
			for(count_pixel2 = setpoint_camera; count_pixel2 <= taos_pixel_max; count_pixel2++)
			{
				if(taos_read_value[count_pixel2] < smallest_min_val_read + range_smallests_values &&
				   taos_read_value[count_pixel2] > smallest_min_val_read - range_smallests_values 	)	
		 	 	{
					if(count_zeros == 0)  {
						edge = count_pixel2;
						flag = 3;
					}
				 	count_zeros = count_zeros + 1;
		 	 	}
			}
			for(count_pixel2 = setpoint_camera; count_pixel2 >= taos_pixel_min; count_pixel2--)
			{
				if(taos_read_value[count_pixel2] < smallest_min_val_read + range_smallests_values &&
				   taos_read_value[count_pixel2] > smallest_min_val_read - range_smallests_values 	)	
				{
					if(count_zeros == 0)  {
						edge = count_pixel2;
						flag = 4;
					}
			 		count_zeros = count_zeros + 1;
				} 
			}
		 }
		 
		count_zeros = 0;
		if (smallest_min_val_read > taos_adc_high_level) {
			edge = taos_pixel_min;
			flag = 5;
		}
	 }
}

/********************************************************************************
						Principal function to camera image processing and
						obtain the error used to servo and dc motors control
********************************************************************************/ 

void TAOS_IMAGE_PROCESS (void)
{ 
	if(count_pixel == taos_pixel_max) 
    {
	if(whitesurface == 0) edge_first_smallest();// better function with colors different of white.
	if((DIP_SW_VAL() & 0x1) != 1 )
		{
			if ((E_ant <= 0.05)&&(E_ant >= -0.05))
			{
				if(FinishLineSearchMode == 0) search_finish_line();
				else if(FinishLineSearchMode == 1) finish_line_detected();
			}
		}
		if(edge <= taos_pixel_min || edge >= taos_pixel_max ) error = E_ant;
		else if(edge >= setpoint_camera) error = ((float)taos_pixel_max - (float)edge)*(-0.30/((float)taos_pixel_max-(float)setpoint_camera));
		else  error = ((float)edge-Threshold)*(0.30 /((float)setpoint_camera-Threshold));
		if(taos_calibration == taos_adc_high_level) error = E_ant;
		if (smallest_min_val_read > taos_adc_high_level) error=E_ant;
		//PTA12_TEST_LOW;
   }	
}
/******************************************************************************
* 	Function to check the start/finish line | - - | (camera and pot2) 
******************************************************************************/

void search_finish_line(void)
{
	
	 zeros_on_track = 0;
	
	 for(count_pixel2 = taos_pixel_min; count_pixel2 <=taos_pixel_max; count_pixel2++)
	 {
		 if(taos_read_value[count_pixel2] <= min_val_read_2 + 30 && 
		    taos_read_value[count_pixel2] >= min_val_read_2 - 30) 
	     {
			 zeros_on_track = zeros_on_track + 1;
	     }
	 }
	 
	 if(zeros_pot >= zeros_on_track - 10 && zeros_pot <= zeros_on_track + 10) finish_flag = 1;
	 //else finish_flag = 0;
	
}

/******************************************************************************
* 	Function to check the start/finish line | - - | (-->) 
******************************************************************************/

void search_finish_line_R(void)
{	
	if((count_pixel2 >= taos_pixel_min + 30 ) && (count_pixel2 <=taos_pixel_max-30))
	{
	switch (count_switch)
	{
	case 0:	
	 		if(taos_read_value[count_pixel2] > calibration_to_zeros) count_1_R++;
	 		else count_switch=1;
	 		break;
	case 1: 		
	 		if(taos_read_value[count_pixel2] < calibration_to_zeros) count_0_R++;
	 		else count_switch=2;
	 		break;
	case 2:
		if(taos_read_value[count_pixel2] > calibration_to_zeros) count_1_R_B++;
		else count_switch=3;
		break;
	default:
			break;
	}
	}
}

/******************************************************************************
* 	Function to check the start/finish line | - - | (<--) 
******************************************************************************/

void search_finish_line_L(void)
{	
	if((count_pixel2 >= taos_pixel_min + 30) && (count_pixel2 <=taos_pixel_max-30))
	{	
	switch (count_switch)
	{
	case 0:	
	 		if(taos_read_value[count_pixel2] > calibration_to_zeros) count_1_L++;
	 		else count_switch=1;
	 		break;
	case 1: 		
	 		if(taos_read_value[count_pixel2] < calibration_to_zeros) count_0_L++;
	 		else count_switch=2;
	 		break;
	case 2:
			if(taos_read_value[count_pixel2] > calibration_to_zeros) count_1_L_B++;
			else count_switch=3;
			break;
	default:
			break;
	}
	}		
}

/******************************************************************************
* 	Function to check the start/finish line | - - |  :)
******************************************************************************/

void finish_line_detected (void)
{
	width_zeros = count_0_L + count_0_R;
	width_ones= count_1_L + count_1_R;
	val_ones_zeros = width_zeros + width_ones; 
	if(     (width_ones >= 14 ) 
			&& (width_ones <=17) 
			&& (count_0_L > 8) 
			&& (count_0_R > 8) 
			&& (count_1_L_B == 0)
			&& (count_1_R_B == 0)) finish_flag = 1;
	if(     (width_zeros >= 19 ) 
			&& (width_ones <=21) 
			&& (count_1_L == 0) 
			&& (count_1_R == 0)
			&& (vision_width > 50)) finish_flag = 1;
	
}
