/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011, 2018 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_main.c
* Version      : CodeGenerator for RL78/G13 V2.05.01.04 [18 May 2018]
* Device(s)    : R5F100LE
* Tool-Chain   : CA78K0R
* Description  : This file implements main function.
* Creation Date: 12/16/2018
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_serial.h"
#include "r_cg_adc.h"
#include "r_cg_timer.h"
#include "r_cg_wdt.h"
/* Start user code for include. Do not edit comment generated here */
 
#include "tsk_cfg.h"
#include "sys_tick.h"
#include "M_zone_handler.h"
#include "A_main_app_handler.h"
#include "M_eeprom_handler.h"
#include "M_rtc_handler.h"
#include "M_sys_config.h"
#include "H_M95M01_eeprom_driver.h"
#include "M_led_handler.h"
#include "M_zone_handler.h"
#include "M_ADC_hdlr.h"
#include "H_DS1307_rtc_driver.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
static uint32_t tick = 0;						// System tick
static TaskType *Task_ptr;                 		// Task pointer
static uint8_t TaskIndex = 0;					// Task index
uint8_t NumTasks =0; 
#ifdef STARTUP_E2P_CLEAR
RTC_DATE_TIME pres_time;
RTC_DATE_TIME verify_time;
#endif
//uint8_t date_time[30]="18/12/14/12/00/00/";
/* End user code. Do not edit comment generated here */
void R_MAIN_UserInit(void);

/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void main(void)
{
    R_MAIN_UserInit();
    /* Start user code. Do not edit comment generated here */
    
    NumTasks = Tsk_GetNumTasks();
    Task_ptr = Tsk_GetConfig();
    
   // for(i=0;i<40;i++)
   //{
	//m_u8LED_ctrl_led(i,ON);  
	//m_u8LED_ctrl_led(i,OFF); 
	//m_u8LED_zone(i,ALT);
   //}
   
    R_UART0_Start();
    R_UART1_Start();
    R_CSI20_Start();
    R_TAU0_Channel0_Start();
   
    R_ADC_Set_OperationOn();
    R_ADC_Start();
    M_ADC_init();
    R_UART1_Send((uint8_t *)"INITIALISING : OK",18);
    //m_u8RTC_strTime_set(date_time);
    
    #ifdef STARTUP_E2P_CLEAR
    m_vE2P_clear();
    pres_time.u8Year=18;
    pres_time.u8Month=12;
    pres_time.u8Day=13;
    pres_time.u8DayOfWeek=0;
    pres_time.u8Hour=16;
    pres_time.u8Minute=56;
    pres_time.u8Second=00;
    h_u8RTC_time_set(pres_time);
    h_u8RTC_time_get(&verify_time);
    #else
    //h_u8RTC_time_get(&verify_time);
    m_u8EEPROM_criticalDataRetrieve();
    P1.6 = ON;
    P4.3 = ON;
    P3.1 = ON;
    #endif           
    while (1U)
    {
	    /**********************OS Scheduler Start *****************************/
	       tick = SysTick_GetSystemTick();		// Get current system tick	      
		// Loop through all tasks.  First, run all continuous tasks.  Then,
		// if the number of ticks since the last time the task was run is
		// greater than or equal to the task interval, execute the task.
		for(TaskIndex = 0; TaskIndex < NumTasks; TaskIndex++)
		{
			if((tick - Task_ptr[TaskIndex].LastTick) >= Task_ptr[TaskIndex].Interval)
			{
				(*Task_ptr[TaskIndex].Func)();         // Execute Task

				
				Task_ptr[TaskIndex].LastTick = tick;  // Save last tick the task was ran.
			}
		}
	   /**********************OS Scheduler End *****************************/		
         
    }
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: R_MAIN_UserInit
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MAIN_UserInit(void)
{
    /* Start user code. Do not edit comment generated here */
    EI();
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
