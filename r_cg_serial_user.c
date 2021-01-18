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
* File Name    : r_cg_serial_user.c
* Version      : CodeGenerator for RL78/G13 V2.05.01.04 [18 May 2018]
* Device(s)    : R5F100LE
* Tool-Chain   : CA78K0R
* Description  : This file implements device driver for Serial module.
* Creation Date: 12/8/2018
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
#pragma interrupt INTST0 r_uart0_interrupt_send
#pragma interrupt INTSR0 r_uart0_interrupt_receive
#pragma interrupt INTST1 r_uart1_interrupt_send
#pragma interrupt INTSR1 r_uart1_interrupt_receive
#pragma interrupt INTCSI20 r_csi20_interrupt
#pragma interrupt INTIIC21 r_iic21_interrupt
#pragma interrupt INTIICA0 r_iica0_interrupt
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_serial.h"
/* Start user code for include. Do not edit comment generated here */
//#include "M_message_queue_handler.h"
//#include "H_M66_gsm_driver.h"
#include "M_LOG_hdlr.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
extern volatile uint8_t * gp_uart0_tx_address;         /* uart0 send buffer address */
extern volatile uint16_t  g_uart0_tx_count;            /* uart0 send data number */
extern volatile uint8_t * gp_uart0_rx_address;         /* uart0 receive buffer address */
extern volatile uint16_t  g_uart0_rx_count;            /* uart0 receive data number */
extern volatile uint16_t  g_uart0_rx_length;           /* uart0 receive data length */
extern volatile uint8_t * gp_uart1_tx_address;         /* uart1 send buffer address */
extern volatile uint16_t  g_uart1_tx_count;            /* uart1 send data number */
extern volatile uint8_t * gp_uart1_rx_address;         /* uart1 receive buffer address */
extern volatile uint16_t  g_uart1_rx_count;            /* uart1 receive data number */
extern volatile uint16_t  g_uart1_rx_length;           /* uart1 receive data length */
extern volatile uint8_t * gp_csi20_rx_address;         /* csi20 receive buffer address */
extern volatile uint16_t  g_csi20_rx_length;           /* csi20 receive data length */
extern volatile uint16_t  g_csi20_rx_count;            /* csi20 receive data count */
extern volatile uint8_t * gp_csi20_tx_address;         /* csi20 send buffer address */
extern volatile uint16_t  g_csi20_send_length;         /* csi20 send data length */
extern volatile uint16_t  g_csi20_tx_count;            /* csi20 send data count */
extern volatile uint8_t   g_iic21_master_status_flag;  /* iic21 start flag for send address check by master mode */
extern volatile uint8_t * gp_iic21_tx_address;         /* iic21 send data pointer by master mode */
extern volatile uint16_t  g_iic21_tx_count;            /* iic21 send data size by master mode */
extern volatile uint8_t * gp_iic21_rx_address;         /* iic21 receive data pointer by master mode */
extern volatile uint16_t  g_iic21_rx_count;            /* iic21 receive data size by master mode */
extern volatile uint16_t  g_iic21_rx_length;           /* iic21 receive data length by master mode */
extern volatile uint8_t   g_iica0_master_status_flag;  /* iica0 master flag */ 
extern volatile uint8_t   g_iica0_slave_status_flag;   /* iica0 slave flag */
extern volatile uint8_t * gp_iica0_rx_address;         /* iica0 receive buffer address */
extern volatile uint16_t  g_iica0_rx_cnt;              /* iica0 receive data length */
extern volatile uint16_t  g_iica0_rx_len;              /* iica0 receive data count */
extern volatile uint8_t * gp_iica0_tx_address;         /* iica0 send buffer address */
extern volatile uint16_t  g_iica0_tx_cnt;              /* iica0 send data count */
/* Start user code for global. Do not edit comment generated here */
#define _40_IICA_PKTLEN_COMPLETE 0x40


volatile uint8_t u8I2C_bridge_rx_flag = 1,u8I2C_bridge_tx_flag = 1;
uint8_t u8_log_uart_tx_flag = 0;
volatile uint16_t tx_ack_cnt = 0,s_cnt = 0,err_cnt = 0;
volatile uint16_t rx_ack_cnt = 0,r_cnt =0;
/***********GSM_UART_VARIABLES*******************/ 
uint8_t h_u8GSM_tx_flag = INT_IDLE,h_u8GSM_rx_flag = INT_IDLE;
int8_t h_u8GSM_End_cnt = 0;
uint8_t eeprom_tx_rx_flag = INT_IDLE;
 
/***********GSM_UART_VARIABLES*******************/

uint8_t h_u8RTC_tx_flag = INT_IDLE;
uint8_t h_u8RTC_rx_flag = INT_IDLE;
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_uart0_interrupt_receive
* Description  : This function is INTSR0 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_uart0_interrupt_receive(void)
{
    volatile uint8_t rx_data;
    volatile uint8_t err_type;
    
    err_type = (uint8_t)(SSR01 & 0x0007U);
    SIR01 = (uint16_t)err_type;

    if (err_type != 0U)
    {
        r_uart0_callback_error(err_type);
    }
    
    rx_data = RXD0;

    if (g_uart0_rx_length > g_uart0_rx_count)
    {
        *gp_uart0_rx_address = rx_data;
        gp_uart0_rx_address++;
        g_uart0_rx_count++;

        if (g_uart0_rx_length == g_uart0_rx_count)
        {
            r_uart0_callback_receiveend();
        }
    }
    else
    {
        r_uart0_callback_softwareoverrun(rx_data);
    }
    
}

/***********************************************************************************************************************
* Function Name: r_uart0_interrupt_send
* Description  : This function is INTST0 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_uart0_interrupt_send(void)
{
    if (g_uart0_tx_count > 0U)
    {
        TXD0 = *gp_uart0_tx_address;
        gp_uart0_tx_address++;
        g_uart0_tx_count--;
    }
    else
    {
        r_uart0_callback_sendend();
    }
}

/***********************************************************************************************************************
* Function Name: r_uart0_callback_receiveend
* Description  : This function is a callback function when UART0 finishes reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_uart0_callback_receiveend(void)
{
    /* Start user code. Do not edit comment generated here */
    
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_uart0_callback_softwareoverrun
* Description  : This function is a callback function when UART0 receives an overflow data.
* Arguments    : rx_data -
*                    receive data
* Return Value : None
***********************************************************************************************************************/
static void r_uart0_callback_softwareoverrun(uint16_t rx_data)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_uart0_callback_sendend
* Description  : This function is a callback function when UART0 finishes transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_uart0_callback_sendend(void)
{
    /* Start user code. Do not edit comment generated here */
    
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_uart0_callback_error
* Description  : This function is a callback function when UART0 reception error occurs.
* Arguments    : err_type -
*                    error type value
* Return Value : None
***********************************************************************************************************************/
static void r_uart0_callback_error(uint8_t err_type)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_uart1_interrupt_receive
* Description  : This function is INTSR1 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_uart1_interrupt_receive(void)
{
    /*volatile uint8_t rx_data;
    volatile uint8_t err_type;
    
    err_type = (uint8_t)(SSR03 & 0x0007U);
    SIR03 = (uint16_t)err_type;

    if (err_type != 0U)
    {
        r_uart1_callback_error(err_type);
    }
    
    rx_data = RXD1;

    if (g_uart1_rx_length > g_uart1_rx_count)
    {
        *gp_uart1_rx_address = rx_data;
        gp_uart1_rx_address++;
        g_uart1_rx_count++;

        if (g_uart1_rx_length == g_uart1_rx_count)
        {
            r_uart1_callback_receiveend();
        }
    }
    else
    {
        r_uart1_callback_softwareoverrun(rx_data);
    }*/
    G_RESP_CODE ret = WAITING;
    uint8_t rx_data;    
    rx_data = RXD1;
    ret = h_u8LOG_Q_send(LOG_QUEUE,(CIRC_BUFF_TYPE *)&rx_data);
    if(ret == MSGQ_FULL)
    {
	  h_u8LOG_Q_send(LOG_QUEUE,(CIRC_BUFF_TYPE *)&rx_data); 
    }
}

/***********************************************************************************************************************
* Function Name: r_uart1_interrupt_send
* Description  : This function is INTST1 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_uart1_interrupt_send(void)
{
    if (g_uart1_tx_count > 0U)
    {
        TXD1 = *gp_uart1_tx_address;
        gp_uart1_tx_address++;
        g_uart1_tx_count--;
    }
    else
    {
        r_uart1_callback_sendend();
    }
}

/***********************************************************************************************************************
* Function Name: r_uart1_callback_receiveend
* Description  : This function is a callback function when UART1 finishes reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_uart1_callback_receiveend(void)
{
    /* Start user code. Do not edit comment generated here */
    u8_log_uart_tx_flag = INT_CMPL;
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_uart1_callback_softwareoverrun
* Description  : This function is a callback function when UART1 receives an overflow data.
* Arguments    : rx_data -
*                    receive data
* Return Value : None
***********************************************************************************************************************/
static void r_uart1_callback_softwareoverrun(uint16_t rx_data)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_uart1_callback_sendend
* Description  : This function is a callback function when UART1 finishes transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_uart1_callback_sendend(void)
{
    /* Start user code. Do not edit comment generated here */
    u8_log_uart_tx_flag = INT_CMPL;
    //u8_log_uart_tx_flag = 1;
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_uart1_callback_error
* Description  : This function is a callback function when UART1 reception error occurs.
* Arguments    : err_type -
*                    error type value
* Return Value : None
***********************************************************************************************************************/
static void r_uart1_callback_error(uint8_t err_type)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_csi20_interrupt
* Description  : This function is INTCSI20 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_csi20_interrupt(void)
{
    volatile uint8_t err_type;
    volatile uint8_t sio_dummy;

    err_type = (uint8_t)(SSR10 & _0001_SAU_OVERRUN_ERROR);
    SIR10 = (uint16_t)err_type;

    if (1U == err_type)
    {
        r_csi20_callback_error(err_type);    /* overrun error occurs */
    }
    else
    {
        if (g_csi20_tx_count > 0U)
        {
            if (0U != gp_csi20_rx_address)
            {
                *gp_csi20_rx_address = SIO20;
                gp_csi20_rx_address++;
            }
            else
            {
                sio_dummy = SIO20;
            }

            if (0U != gp_csi20_tx_address)
            {
                SIO20 = *gp_csi20_tx_address;
                gp_csi20_tx_address++;
            }
            else
            {
                SIO20 = 0xFFU;
            }

            g_csi20_tx_count--;
        }
        else 
        {
            if (0U == g_csi20_tx_count)
            {
                if (0U != gp_csi20_rx_address)
                {
                    *gp_csi20_rx_address = SIO20;
                }
                else
                {
                    sio_dummy = SIO20;
                }
            }

            r_csi20_callback_sendend();    /* complete send */
            r_csi20_callback_receiveend();    /* complete receive */
        }
    }
}

/***********************************************************************************************************************
* Function Name: r_csi20_callback_receiveend
* Description  : This function is a callback function when CSI20 finishes reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_csi20_callback_receiveend(void)
{
    /* Start user code. Do not edit comment generated here */
    eeprom_tx_rx_flag = INT_CMPL;
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_csi20_callback_error
* Description  : This function is a callback function when CSI20 reception error occurs.
* Arguments    : err_type -
*                    error type value
* Return Value : None
***********************************************************************************************************************/
static void r_csi20_callback_error(uint8_t err_type)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_csi20_callback_sendend
* Description  : This function is a callback function when CSI20 finishes transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_csi20_callback_sendend(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_iic21_interrupt
* Description  : This function is INTIIC21 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_iic21_interrupt(void)
{
    if (((SSR11 & _0002_SAU_PARITY_ERROR) == 0x0002U) && (g_iic21_tx_count != 0U))
    {
        r_iic21_callback_master_error(MD_NACK);
    }
    else if(((SSR11 & _0001_SAU_OVERRUN_ERROR) == 0x0001U) && (g_iic21_tx_count != 0U))
    {
        r_iic21_callback_master_error(MD_OVERRUN);
    }
    else
    {
        /* Control for master send */
        if ((g_iic21_master_status_flag & _01_SAU_IIC_SEND_FLAG) == 1U)
        {
            if (g_iic21_tx_count > 0U)
            {
                SIO21 = *gp_iic21_tx_address;
                gp_iic21_tx_address++;
                g_iic21_tx_count--;
            }
            else
            {
                R_IIC21_StopCondition();
                r_iic21_callback_master_sendend();
            }
        }
        /* Control for master receive */
        else 
        {
            if ((g_iic21_master_status_flag & _04_SAU_IIC_SENDED_ADDRESS_FLAG) == 0U)
            {
                ST1 |= _0002_SAU_CH1_STOP_TRG_ON;
                SCR11 &= ~_C000_SAU_RECEPTION_TRANSMISSION;
                SCR11 |= _4000_SAU_RECEPTION;
                SS1 |= _0002_SAU_CH1_START_TRG_ON;
                g_iic21_master_status_flag |= _04_SAU_IIC_SENDED_ADDRESS_FLAG;
                
                if (g_iic21_rx_length == 1U)
                {
                    SOE1 &= ~_0002_SAU_CH1_OUTPUT_ENABLE;    /* disable IIC21 out */
                }
                
                SIO21 = 0xFFU;
            }
            else
            {
                if (g_iic21_rx_count < g_iic21_rx_length)
                {
                    *gp_iic21_rx_address = SIO21;
                    gp_iic21_rx_address++;
                    g_iic21_rx_count++;
                    
                    if (g_iic21_rx_count == (g_iic21_rx_length - 1U))
                    {
                        SOE1 &= ~_0002_SAU_CH1_OUTPUT_ENABLE;    /* disable IIC21 out */
                        SIO21 = 0xFFU;
                    }
                    else if (g_iic21_rx_count == g_iic21_rx_length)
                    {
                        R_IIC21_StopCondition();
                        r_iic21_callback_master_receiveend();
                    }
                    else
                    {
                        SIO21 = 0xFFU;
                    }
                }
            }
        }
    }
}

/***********************************************************************************************************************
* Function Name: r_iic21_callback_master_error
* Description  : This function is a callback function when IIC21 master error occurs.
* Arguments    : flag -
*                    status flag
* Return Value : None
***********************************************************************************************************************/
static void r_iic21_callback_master_error(MD_STATUS flag)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_iic21_callback_master_receiveend
* Description  : This function is a callback function when IIC21 finishes master reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_iic21_callback_master_receiveend(void)
{
    /* Start user code. Do not edit comment generated here */
    h_u8RTC_rx_flag = INT_CMPL;
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_iic21_callback_master_sendend
* Description  : This function is a callback function when IIC21 finishes master transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_iic21_callback_master_sendend(void)
{
    /* Start user code. Do not edit comment generated here */
    h_u8RTC_tx_flag = INT_CMPL;
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_iica0_interrupt
* Description  : This function is INTIICA0 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_iica0_interrupt(void)
{
    if ((IICS0 & _80_IICA_STATUS_MASTER) == 0x80U)
    {
        iica0_master_handler();
    }
}

/***********************************************************************************************************************
* Function Name: iica0_master_handler
* Description  : This function is IICA0 master handler.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void iica0_master_handler(void)
{
    /* Control for communication */
    if ((0U == IICBSY0) && (g_iica0_tx_cnt != 0U))
    {
        r_iica0_callback_master_error(MD_SPT);
    }
    /* Control for sended address */
    else
    {
        if ((g_iica0_master_status_flag & _80_IICA_ADDRESS_COMPLETE) == 0U)
        {
            if (1U == ACKD0)
            {
                g_iica0_master_status_flag |= _80_IICA_ADDRESS_COMPLETE;
                
                if (1U == TRC0)
                {
                    WTIM0 = 1U;
                    
                    if (g_iica0_tx_cnt > 0U)
                    {
                        IICA0 = *gp_iica0_tx_address;
                        gp_iica0_tx_address++;
                        g_iica0_tx_cnt--;
                    }
                    else
                    {
                        r_iica0_callback_master_sendend();
                    }
                }
                else
                {
                    ACKE0 = 1U;
                    WTIM0 = 0U;
                    WREL0 = 1U;
                }
            }
            else
            {
                r_iica0_callback_master_error(MD_NACK);
            }
        }
        else
        {
            /* Master send control */
            if (1U == TRC0)
            {
                if ((0U == ACKD0) && (g_iica0_tx_cnt != 0U))
                {
                    r_iica0_callback_master_error(MD_NACK);
                }
                else
                {
                    if (g_iica0_tx_cnt > 0U)
                    {
                        IICA0 = *gp_iica0_tx_address;
                        gp_iica0_tx_address++;
                        g_iica0_tx_cnt--;
                    }
                    else
                    {
                        r_iica0_callback_master_sendend();
                    }
                }
            }
            /* Master receive control */
            else
            {
                if (g_iica0_rx_cnt < g_iica0_rx_len)
                {
                    *gp_iica0_rx_address = IICA0;
                    gp_iica0_rx_address++;
                    g_iica0_rx_cnt++;
                    
                    if (g_iica0_rx_cnt == g_iica0_rx_len)
                    {
                        ACKE0 = 0U;
                        WTIM0 = 1U;
                        WREL0 = 1U;
                    }
                    else
                    {
                        WREL0 = 1U;
                    }
                }
                else
                {
                    r_iica0_callback_master_receiveend();
                }
            }
        }
    }
}

/***********************************************************************************************************************
* Function Name: r_iica0_callback_master_error
* Description  : This function is a callback function when IICA0 master error occurs.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_iica0_callback_master_error(MD_STATUS flag)
{
    /* Start user code. Do not edit comment generated here */
    err_cnt++;     
    u8I2C_bridge_tx_flag  =1;
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_iica0_callback_master_receiveend
* Description  : This function is a callback function when IICA0 finishes master reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_iica0_callback_master_receiveend(void)
{
    SPT0 = 1U;
    /* Start user code. Do not edit comment generated here */
    u8I2C_bridge_rx_flag  =1;
    //m_u8MSGQ_I2C_Rx_callback();
    r_cnt++;
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_iica0_callback_master_sendend
* Description  : This function is a callback function when IICA0 finishes master transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_iica0_callback_master_sendend(void)
{
    SPT0 = 1U;
    /* Start user code. Do not edit comment generated here */
    u8I2C_bridge_tx_flag  =1;
    s_cnt++;
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
