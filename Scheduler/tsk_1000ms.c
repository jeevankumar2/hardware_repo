/** \file tsk_1000ms.c
 * \brief This module contains 1000 ms periodic tasks.
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
#include "i_GSM.h"
#include "i_TCP.h"
#include "r_cg_userdefine.h"

//#include "i_GPS.h"
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
*  This function is used for the 1000 ms tasks.
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

uint8_t u8_GPSPacktime_count=0;
uint8_t u8_Serverupdatetime_count=0;

uint8_t u8_Serverupdatetime_inSec =0;
uint8_t u8GPSPack_data_Int=0;

void Tsk_1000ms(void)
{ 

	if(u8_start_gps_updatetask==1)
	{
	 u8_GPSPacktime_count++;
	 u8_Serverupdatetime_count++;
	 //if(u8_time_count==GPS_PACKING_TIME && TCP_process_status!=0)
//	 if(u8_GPSPacktime_count>=GPS_PACKING_TIME && TCP_process_status==0)
//	 {
//	  (void)TCP_pack_data();
//	  u8_GPSPacktime_count=0;
//	 }
	 /*****10 sec server update time*********/
	 if(u8_Serverupdatetime_count>=10)
	 {
	   DI();
	  read_gpio();
	  GPS_process();
	  (void)TCP_pack_data();//Pack the fleet packet datas
	  TCP_process_status=1;
	  u8GPSPack_data_Int=1;
	  u8_Serverupdatetime_count=0;
	   EI();
	 }
	}


}
/****************** End of File **************************************************************/

