/** \file tsk_200ms.c
 * \brief This module contains 200 ms periodic tasks.
 *
 *  This module provides the functionality related to tasks that are scheduled
 *  every 100 ms.  Functions that need to be ran in this interval can be added to
 *  task in this module.  The main function will call this task at the pre-
 *  determined interval and all the code within it.  If the task needs to be
 *  ran continuously it should be placed in tsk().
 */
/******************************************************************************
* Includes
*******************************************************************************/
#include "tsk_200ms.h"			// For this modules definitions


/******************************************************************************
* Constants
*******************************************************************************/

/******************************************************************************
* Typedefs
*******************************************************************************/

/******************************************************************************
* Variable Definitions
*******************************************************************************/

/******************************************************************************
* Function Prototypes
*******************************************************************************/

/******************************************************************************
* Function Definitions
*******************************************************************************/
/********************************************************************************
* Function : Tsk_200ms
*//**
* \section Description Description:
*
*  This function is used for the 100 ms tasks.
*
* \param        None.
* \return       None.
*
* \see Tsk
* \see Tsk_10ms
*
*  ----------------------
*  - HISTORY OF CHANGES -
*  ----------------------
*    Date    Software Version    Initials   Description
*         0.1.1                  Function Created.
*
*******************************************************************************/
void Tsk_200ms(void)
{
  Task_GSM_Handler();
  R_WDT_Restart();
}

/*** End of File **************************************************************/

