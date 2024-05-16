/*============================================================================*/
/*                     AZOR - EMBEDDED SYSTEMS SOFTWARE                       */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*
* C Source:        periodic_tasks_exec.c
* version:         1.0 
* created_by:      MDRR
* date_created:    July 31 2013 
*=============================================================================*/
/* DESCRIPTION :  This file contains the periodic tasks definition.           */
/*============================================================================*/
/* FUNCTION COMMENT : Add here the user functionality, inside a periodic task */
/* according to the desired execution period.                                 */
/*                                                                            */
/*============================================================================*/
/*                               OBJECT HISTORY                               */
/*============================================================================*/
/*  REVISION |   DATE      |                               |      AUTHOR      */
/*----------------------------------------------------------------------------*/
/*  1.0      | 31/jul/2013 |                               | MDRR             */
/* This file is created based on apptask module from the old version of this  */
/* scheduler.                                                                 */
/*============================================================================*/

/* Includes */
/* -------- */
#include "periodic_tasks_exec.h"
#include "periodic_tasks_modules.h"


/**************************************************************
 *  Name                 : periodic_tasks_exec_2tks
 *	ID					 : TASK_2TKS
 *  Description          : Container for functionality that is 
                           executed periodically.
 *  Parameters           : none
 *  Return               : none
 *  Critical/explanation : no
 **************************************************************/
 void periodic_tasks_exec_2tks(void)
 {
	 TAOS_CLK_SI_SIGNALS();
 }
 
 /**************************************************************
 *  Name                 : periodic_tasks_exec_524tks
 *	ID					 : TASK_524TKS
 *  Description          : Container for functionality that is 
                           executed periodically.
 *  Parameters           : none
 *  Return               : none
 *  Critical/explanation : no
 **************************************************************/
 void periodic_tasks_exec_524tks(void)
 {
	 SERVO_DIRECTION_CONTROL();
	 DC_MOTORS_CONTROL();
 }
 
 /**************************************************************
 *  Name                 : periodic_tasks_exec_20tks
 *	ID					 : TASK_20TKS
 *  Description          : Container for functionality that is 
                           executed periodically.
 *  Parameters           : none
 *  Return               : none
 *  Critical/explanation : no
 **************************************************************/
 void periodic_tasks_exec_20tks(void)
 {

 }
 
 /**************************************************************
 *  Name                 : periodic_tasks_exec_50tks
 *	ID					 : TASK_50TKS
 *  Description          : Container for functionality that is 
                           executed periodically.
 *  Parameters           : none
 *  Return               : none
 *  Critical/explanation : no
 **************************************************************/
 void periodic_tasks_exec_50tks(void)
 {
	
 }
 
 /**************************************************************
 *  Name                 : periodic_tasks_exec_100tks
 *	ID					 : TASK_100TKS
 *  Description          : Container for functionality that is 
                           executed periodically.
 *  Parameters           : none
 *  Return               : none
 *  Critical/explanation : no
 **************************************************************/
 void periodic_tasks_exec_100tks(void)
 { 
	
 }
 
 /**************************************************************
 *  Name                 : periodic_tasks_exec_500tks
 *	ID					 : TASK_500TKS
 *  Description          : Container for functionality that is 
                           executed periodically.
 *  Parameters           : none
 *  Return               : none
 *  Critical/explanation : no
 **************************************************************/
 void periodic_tasks_exec_500tks(void)
 {

 }
 
 /**************************************************************
 *  Name                 : periodic_tasks_exec_1000tks
 *	ID					 : TASK_1000TKS
 *  Description          : Container for functionality that is 
                           executed periodically.
 *  Parameters           : none
 *  Return               : none
 *  Critical/explanation : no
 **************************************************************/
 void periodic_tasks_exec_1000tks(void)
 {
	
 }
 
