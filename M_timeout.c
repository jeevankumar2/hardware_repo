/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by IELEKTRON TECHNOLOGIES
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
*
* Copyright (C) 2017, ielektron technologies. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : keypad.c
* Version      : CodeGenerator for RL78/G13 V2.03.02.01 [15 May 2015]
* Device(s)    : R5F100FE
* Tool-Chain   : CA78K0R
* Description  : This file implements main function.
* Creation Date: 8/25/2017
***********************************************************************************************************************/

#include "M_time_out.h"
#include "r_cg_serial.h"
#include "r_cg_timer.h"

 

extern uint8_t u8_timeoutdelay_count; 

void timeout_start(uint8_t timeout_val)
{	 
	if(u8_timeoutdelay_count > 0)
	{
		R_TAU0_Channel3_Stop();	 	 
	}
	R_TAU0_Channel3_Start();
	u8_timeoutdelay_count = timeout_val; 
}

void timeout_stop(void)
{
	R_TAU0_Channel3_Stop();	 
	u8_timeoutdelay_count=0;
}

uint8_t is_timeout_expired(void)
{
	if(u8_timeoutdelay_count <= 0)
	{
		return EXPIRED;	
	}
	else
	{
		return NOT_EXPIRED;	
	}
}
uint8_t timeout_chk(uint8_t *flag,uint8_t code,uint8_t timeout)
{
	uint8_t ret = NOT_EXPIRED;
	timeout_start(timeout);
	while((*flag == INT_IDLE) && (!is_timeout_expired()));
	if(EXPIRED == is_timeout_expired())
	{
	  if(code != 100)
	  {
		  //m_u8MSGQ_cmd_send(LCD_DBG_CMD,code,FP_QUEUE);
	  }	  
	  ret = EXPIRED;
	}
	timeout_stop();
	*flag = INT_IDLE;
	return ret;
}
