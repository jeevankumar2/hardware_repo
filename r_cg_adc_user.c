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
* File Name    : r_cg_adc_user.c
* Version      : CodeGenerator for RL78/G13 V2.05.01.04 [18 May 2018]
* Device(s)    : R5F100LE
* Tool-Chain   : CA78K0R
* Description  : This file implements device driver for ADC module.
* Creation Date: 12/16/2018
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
#pragma interrupt INTAD r_adc_interrupt
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_adc.h"
/* Start user code for include. Do not edit comment generated here */
#include "M_ADC_hdlr.h"
#include "M_event_handler.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
extern ADC_CH_ST ADC_ch_st[NO_OF_ADC_IN];
extern uint8_t ADC_CH_cnt;
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_adc_interrupt
* Description  : This function is INTAD interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_adc_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
     if(!GSM_event_flag_manager(ADC_CRI_FLAG,FLAG_CHK))
	    {
		    /*Get conversion result of the current adc channel**/    
	            R_ADC_Get_Result(&ADC_ch_st[ADC_CH_cnt].RAW_ADC_buffer);		     
	    }
	     
	     /*Increment for next channel***/
	     ADC_CH_cnt++; 
	     if(ADC_CH_cnt == MAX_ADC_CH_SCAN)
	     {
		    ADS = _13_AD_INPUT_CHANNEL_19; 
	     }
	     else if(ADC_CH_cnt >= TOTAL_AD_CH)
	     {
		     ADC_CH_cnt = 0;
		     ADS = _12_AD_INPUT_CHANNEL_18;
	     }
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
