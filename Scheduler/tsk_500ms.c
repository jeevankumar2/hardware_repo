/** \file tsk_500ms.c
 * \brief This module contains 500 ms periodic tasks.
 *
 *  This module provides the functionality related to tasks that are scheduled
 *  every 500 ms.  Functions that need to be ran in this interval can be added to
 *  task in this module.  The main function will call this task at the pre-
 *  determined interval and all the code within it.  If the task needs to be
 *  ran continuously it should be placed in tsk().
 */
/******************************************************************************
* Includes
*******************************************************************************/
 #include "M_LOG_hdlr.h"
 #include "r_cg_wdt.h"
 #include "A_main_app_handler.h"
 #include "M_ADC_hdlr.h"
 #include "r_cg_serial.h"
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
* Function : Tsk_100ms
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
void Tsk_500ms(void)
{      
   M_ADCMonitor();
   a_u8APP_handler();
   LOG_Hdlr();
   R_WDT_Restart();
   //R_UART1_Send((uint8_t *)"INITIALISING : OK",18);
}

/*** End of File **************************************************************/

