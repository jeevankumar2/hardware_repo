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
* File Name    : r_cg_userdefine.h
* Version      : CodeGenerator for RL78/G13 V2.05.01.04 [18 May 2018]
* Device(s)    : R5F100LE
* Tool-Chain   : CA78K0R
* Description  : This file includes user definition.
* Creation Date: 12/16/2018
***********************************************************************************************************************/

#ifndef _USER_DEF_H
#define _USER_DEF_H

/***********************************************************************************************************************
User definitions
***********************************************************************************************************************/

/* Start user code for function. Do not edit comment generated here */

/****************LESSON LEARNT
1.R_ADC_Set_OperationOn FN SHOULD BE CALLED BEFORE R_ADC_Start().
******************************/
#include "stdio.h"
//#define DEBUG_ENABLE

//#define STARTUP_E2P_CLEAR

#ifdef DEBUG_ENABLE
#define DEBUG_LEN 30 
extern char debug_arr[DEBUG_LEN]; 
extern uint8_t u8_log_uart_tx_flag;
#endif

#define MAX_U16_VAL 25000
#define MIN_IN_SEC 120
#define ZERO_ASCII_VAL 48
/*
typedef enum
{
	SUCCESS = 0,
	FAILURE,
	WAITING,
	INVALID_PARAM,
	NO_RESPONSE,
	MSGQ_FULL,
	MSGQ_EMPTY,
	MSGQ_NOT_ENOUGH,
	MSGQ_SUCCESS,
	MSGQ_FAILED,
	MSGQ_MEM_FULL,//when the message memory is full
	MSG_EEPROM_DATA_INVALID, //when either the packet len,packet id,split size is invalid or not within the acceptable range
	FREE,
	NOT_FREE,
	EEPROM_IN_PRO,	 
	//if data buff is occupied and the received message needs the data buffer then this response will be sent
	DAT_BUF_NOT_AVAILABLE,
	MSGQ_TIMEOUT,
	CALL_HANG,
	CALL_ESTB,
	CALL_ANS,
	CALL_ANS_ERROR,
	EXIT,
	GSM_INIT_COMPLETE,
	GSM_INIT_FAILED,
	GSM_INT_ERROR,
	GSM_NO_SIM,
	GSM_SIM_OK,
	GSM_OK,
	GSM_UART_OK,
	GSM_UART_ERROR,
	GSM_OFF,
	GSM_TIMEOUT_ERROR,
	GSM_INITIALIZING,
	POW_AC,
	POW_NORMAL,
	POW_LOW,
	POW_SHUT,
	E2P_OK,
	E2P_NOT_OK,
	RTC_OK,
	RTC_NOT_OK,
	RESP_RX,
	MSMS_WAIT,
	MSMS_SUCC,
	MSMS_FAIL,
	SYS_DISABLED,
	SMS_NIGHT,
	SMS_DAY,
	SMS_DISABLE,
	LED_OK,
	LOG_START,
	LOG_STP,
	SYS_ENABLE,
	SYS_DISABLE,
	DATA_AVAILABLE,
	SYS_IN_ALT,
	SYS_SAFE,
}G_RESP_CODE;

*/


typedef enum
{
	SUCCESS = 0,
	FAILURE,
	WAITING,
	MSGQ_FULL,
	MSGQ_EMPTY,
	LOG_START,
	LOG_STP,
	DATA_AVAILABLE,
	SYS_IN_ALT,
	SYS_SAFE,
}G_RESP_CODE;




typedef enum
{
	OFF = 0, // OFF REPRESENTS 0V SO DONT  CHANGE THE EENUM VALUE
	ON = 1,  // ON REPRESENTS 5V SO DONT  CHANGE THE EENUM VALUE
	NILL = 2,
	GET = 3,
	T_START = 4, // USED TO START THE TIMER
	T_STOP = 5, // USED TO STOP THE TIMER
	T_RESTART = 6,//USED TO RESTART THE TIMER
	RESET = 7,	
 	SET = 8,
	CLR = 9,
	CHK = 10,
	RTC_STORE = 11
}G_CTL_CODES; 


#define SL_ADDR         (0xA0U)         /* Slave address */

/***********************************E2P INDEX DEFINITIONS START***********************************************/

#define PWD_START_INDEX 0
#define PWD_OFFSET 4

#define CURR_E2P_PAGE 4
#define CURR_E2P_PAGE_OFFSET 2

/***********************************E2P INDEX DEFINITIONS END***********************************************/
/* End user code. Do not edit comment generated here */
#endif
