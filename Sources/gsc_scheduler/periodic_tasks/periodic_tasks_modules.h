/*============================================================================*/
/*                     AZOR - EMBEDDED SYSTEMS SOFTWARE                       */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*
* C Include:       periodic_tasks_modules.h
* version:         1.0 
* created_by:      MDRR 
* date_created:    July 31 2013     
*=============================================================================*/
/* DESCRIPTION : List of headers of the modules executed from some of the     */
/* periodic tasks defined in periodic_tasks_exec.c                            */                      
/*============================================================================*/
/* FUNCTION COMMENT : In the list of this header file the user must add the   */
/* header name of the functionality that have to be executed periodically      */
/*                                                                            */
/*============================================================================*/
/*                               OBJECT HISTORY                               */
/*============================================================================*/
/*  REVISION |   DATE      |                               |      AUTHOR      */
/*----------------------------------------------------------------------------*/
/*  1.0      | 31/jul/2013 |                               | MDRR             */
/*  Creation of this file                                                     */
/*============================================================================*/

#ifndef PERIODIC_TASKS_MODULES_H                 /* To avoid double inclusion */
#define PERIODIC_TASKS_MODULES_H

/*============================================================================*/
/* List of headers of the modules executed from some of these periodic tasks  */
/*============================================================================*/
//#include "MyApplication.h"
//#include "MyLowLayerDriver.h"

/*************************************/
/*  Includes from MCU drivers        */
/*************************************/
#include "..\..\mcl\ADC.h"
#include "..\..\mcl\tpm.h"
#include "..\..\mcl\GPIO.h"

/*************************************/

/*********************************************/
/*  Includes from extern hardware drivers    */
/*********************************************/
#include "..\..\sdl\KL25Z_SHIELD_INTERFACE.h"
/*********************************************/

/*************************************************************/
/*  Includes from hardware application source codes          */
/*************************************************************/
#include "..\..\app\LineScanCamera.h"
#include "..\..\app\SERVO.h"
#include "..\..\app\DC_Motors_Control.h"
#include "..\..\app\Speedsensors.h"
/*************************************************************/
/*============================================================================*/

#endif


