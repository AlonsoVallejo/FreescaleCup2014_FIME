/*
 * tpm.h
 *
 *  Created on: Jul 16, 2013
 *      Author: B34443
 */

#ifndef TPM_H_
#define TPM_H_

/********************************************************************************
						libraries used.
********************************************************************************/ 
#include "derivative.h"

/********************************************************************************
						Functions used.
********************************************************************************/ 
void INIT_SERVO(void);
void SERVO_POSITION(float DutyCycle);
void INIT_PWML_MOTOR_1 (void);
void INIT_PWML_MOTOR_2 (void);
void INIT_PWMR_MOTOR_1 (void);
void INIT_PWMR_MOTOR_2 (void);
void MOTOR1_R_SPEED(unsigned int PWM_MOTOR);
void MOTOR2_R_SPEED(unsigned int PWM_MOTOR);
void MOTOR1_L_SPEED(unsigned int PWM_MOTOR);
void MOTOR2_L_SPEED(unsigned int PWM_MOTOR);
void INIT_PWM_LEDS_CAMERA(void);
void LEDS_CAMERA_VALUE(unsigned int TAOS_LEDS_VALUE);

#endif /* TPM_H_ */
