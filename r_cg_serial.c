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
* File Name    : r_cg_serial.c
* Version      : CodeGenerator for RL78/G13 V2.05.01.04 [18 May 2018]
* Device(s)    : R5F100LE
* Tool-Chain   : CA78K0R
* Description  : This file implements device driver for Serial module.
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
#include "r_cg_serial.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
volatile uint8_t * gp_uart0_tx_address;        /* uart0 transmit buffer address */
volatile uint16_t  g_uart0_tx_count;           /* uart0 transmit data number */
volatile uint8_t * gp_uart0_rx_address;        /* uart0 receive buffer address */
volatile uint16_t  g_uart0_rx_count;           /* uart0 receive data number */
volatile uint16_t  g_uart0_rx_length;          /* uart0 receive data length */
volatile uint8_t * gp_uart1_tx_address;        /* uart1 transmit buffer address */
volatile uint16_t  g_uart1_tx_count;           /* uart1 transmit data number */
volatile uint8_t * gp_uart1_rx_address;        /* uart1 receive buffer address */
volatile uint16_t  g_uart1_rx_count;           /* uart1 receive data number */
volatile uint16_t  g_uart1_rx_length;          /* uart1 receive data length */
volatile uint8_t * gp_csi20_rx_address;        /* csi20 receive buffer address */
volatile uint16_t  g_csi20_rx_length;          /* csi20 receive data length */
volatile uint16_t  g_csi20_rx_count;           /* csi20 receive data count */
volatile uint8_t * gp_csi20_tx_address;        /* csi20 send buffer address */
volatile uint16_t  g_csi20_send_length;        /* csi20 send data length */
volatile uint16_t  g_csi20_tx_count;           /* csi20 send data count */
volatile uint8_t   g_iic21_master_status_flag; /* iic21 start flag for send address check by master mode */
volatile uint8_t * gp_iic21_tx_address;        /* iic21 send data pointer by master mode */
volatile uint16_t  g_iic21_tx_count;           /* iic21 send data size by master mode */
volatile uint8_t * gp_iic21_rx_address;        /* iic21 receive data pointer by master mode */
volatile uint16_t  g_iic21_rx_count;           /* iic21 receive data size by master mode */
volatile uint16_t  g_iic21_rx_length;          /* iic21 receive data length by master mode */
volatile uint8_t   g_iica0_master_status_flag; /* iica0 master flag */
volatile uint8_t   g_iica0_slave_status_flag;  /* iica0 slave flag */
volatile uint8_t * gp_iica0_rx_address;        /* iica0 receive buffer address */
volatile uint16_t  g_iica0_rx_len;             /* iica0 receive data length */
volatile uint16_t  g_iica0_rx_cnt;             /* iica0 receive data count */
volatile uint8_t * gp_iica0_tx_address;        /* iica0 send buffer address */
volatile uint16_t  g_iica0_tx_cnt;             /* iica0 send data count */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_SAU0_Create
* Description  : This function initializes the SAU0 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_SAU0_Create(void)
{
    SAU0EN = 1U;    /* supply SAU0 clock */
    NOP();
    NOP();
    NOP();
    NOP();
    SPS0 = _0002_SAU_CK00_FCLK_2 | _0020_SAU_CK01_FCLK_2;
    R_UART0_Create();
    R_UART1_Create();
}

/***********************************************************************************************************************
* Function Name: R_UART0_Create
* Description  : This function initializes the UART0 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_UART0_Create(void)
{
    ST0 |= _0002_SAU_CH1_STOP_TRG_ON | _0001_SAU_CH0_STOP_TRG_ON;    /* disable UART0 receive and transmit */
    STMK0 = 1U;    /* disable INTST0 interrupt */
    STIF0 = 0U;    /* clear INTST0 interrupt flag */
    SRMK0 = 1U;    /* disable INTSR0 interrupt */
    SRIF0 = 0U;    /* clear INTSR0 interrupt flag */
    SREMK0 = 1U;   /* disable INTSRE0 interrupt */
    SREIF0 = 0U;   /* clear INTSRE0 interrupt flag */
    /* Set INTST0 low priority */
    STPR10 = 1U;
    STPR00 = 1U;
    /* Set INTSR0 low priority */
    SRPR10 = 1U;
    SRPR00 = 1U;
    SMR00 = _0020_SAU_SMRMN_INITIALVALUE | _0000_SAU_CLOCK_SELECT_CK00 | _0000_SAU_TRIGGER_SOFTWARE |
            _0002_SAU_MODE_UART | _0000_SAU_TRANSFER_END;
    SCR00 = _8000_SAU_TRANSMISSION | _0000_SAU_INTSRE_MASK | _0000_SAU_PARITY_NONE | _0080_SAU_LSB | _0010_SAU_STOP_1 |
            _0007_SAU_LENGTH_8;
    SDR00 = _CE00_UART0_TRANSMIT_DIVISOR;
    NFEN0 |= _01_SAU_RXD0_FILTER_ON;
    SIR01 = _0004_SAU_SIRMN_FECTMN | _0002_SAU_SIRMN_PECTMN | _0001_SAU_SIRMN_OVCTMN;    /* clear error flag */
    SMR01 = _0020_SAU_SMRMN_INITIALVALUE | _0000_SAU_CLOCK_SELECT_CK00 | _0100_SAU_TRIGGER_RXD | _0000_SAU_EDGE_FALL |
            _0002_SAU_MODE_UART | _0000_SAU_TRANSFER_END;
    SCR01 = _4000_SAU_RECEPTION | _0000_SAU_INTSRE_MASK | _0000_SAU_PARITY_NONE | _0080_SAU_LSB | _0010_SAU_STOP_1 |
            _0007_SAU_LENGTH_8;
    SDR01 = _CE00_UART0_RECEIVE_DIVISOR;
    SO0 |= _0001_SAU_CH0_DATA_OUTPUT_1;
    SOL0 |= _0000_SAU_CHANNEL0_NORMAL;    /* output level normal */
    SOE0 |= _0001_SAU_CH0_OUTPUT_ENABLE;    /* enable UART0 output */
    /* Set RxD0 pin */
    PM1 |= 0x02U;
    /* Set TxD0 pin */
    P1 |= 0x04U;
    PM1 &= 0xFBU;
}

/***********************************************************************************************************************
* Function Name: R_UART0_Start
* Description  : This function starts the UART0 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_UART0_Start(void)
{
    SO0 |= _0001_SAU_CH0_DATA_OUTPUT_1;    /* output level normal */
    SOE0 |= _0001_SAU_CH0_OUTPUT_ENABLE;    /* enable UART0 output */
    SS0 |= _0002_SAU_CH1_START_TRG_ON | _0001_SAU_CH0_START_TRG_ON;    /* enable UART0 receive and transmit */
    STIF0 = 0U;    /* clear INTST0 interrupt flag */
    SRIF0 = 0U;    /* clear INTSR0 interrupt flag */
    STMK0 = 0U;    /* enable INTST0 interrupt */
    SRMK0 = 0U;    /* enable INTSR0 interrupt */
}

/***********************************************************************************************************************
* Function Name: R_UART0_Stop
* Description  : This function stops the UART0 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_UART0_Stop(void)
{
    STMK0 = 1U;    /* disable INTST0 interrupt */
    SRMK0 = 1U;    /* disable INTSR0 interrupt */
    ST0 |= _0002_SAU_CH1_STOP_TRG_ON | _0001_SAU_CH0_STOP_TRG_ON;    /* disable UART0 receive and transmit */
    SOE0 &= ~_0001_SAU_CH0_OUTPUT_ENABLE;    /* disable UART0 output */
    STIF0 = 0U;    /* clear INTST0 interrupt flag */
    SRIF0 = 0U;    /* clear INTSR0 interrupt flag */
   
}

/***********************************************************************************************************************
* Function Name: R_UART0_Receive
* Description  : This function receives UART0 data.
* Arguments    : rx_buf -
*                    receive buffer pointer
*                rx_num -
*                    buffer size
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/
MD_STATUS R_UART0_Receive(uint8_t * const rx_buf, uint16_t rx_num)
{
    MD_STATUS status = MD_OK;

    if (rx_num < 1U)
    {
        status = MD_ARGERROR;
    }
    else
    {
        g_uart0_rx_count = 0U;
        g_uart0_rx_length = rx_num;
        gp_uart0_rx_address = rx_buf;
    }

    return (status);
}

/***********************************************************************************************************************
* Function Name: R_UART0_Send
* Description  : This function sends UART0 data.
* Arguments    : tx_buf -
*                    transfer buffer pointer
*                tx_num -
*                    buffer size
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/
MD_STATUS R_UART0_Send(uint8_t * const tx_buf, uint16_t tx_num)
{
    MD_STATUS status = MD_OK;

    if (tx_num < 1U)
    {
        status = MD_ARGERROR;
    }
    else
    {
        gp_uart0_tx_address = tx_buf;
        g_uart0_tx_count = tx_num;
        STMK0 = 1U;    /* disable INTST0 interrupt */
        TXD0 = *gp_uart0_tx_address;
        gp_uart0_tx_address++;
        g_uart0_tx_count--;
        STMK0 = 0U;    /* enable INTST0 interrupt */
    }

    return (status);
}

/***********************************************************************************************************************
* Function Name: R_UART1_Create
* Description  : This function initializes the UART1 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_UART1_Create(void)
{
    ST0 |= _0008_SAU_CH3_STOP_TRG_ON | _0004_SAU_CH2_STOP_TRG_ON;    /* disable UART1 receive and transmit */
    STMK1 = 1U;    /* disable INTST1 interrupt */
    STIF1 = 0U;    /* clear INTST1 interrupt flag */
    SRMK1 = 1U;    /* disable INTSR1 interrupt */
    SRIF1 = 0U;    /* clear INTSR1 interrupt flag */
    SREMK1 = 1U;   /* disable INTSRE1 interrupt */
    SREIF1 = 0U;   /* clear INTSRE1 interrupt flag */
    /* Set INTST1 low priority */
    STPR11 = 1U;
    STPR01 = 1U;
    /* Set INTSR1 low priority */
    SRPR11 = 1U;
    SRPR01 = 1U;
    SMR02 = _0020_SAU_SMRMN_INITIALVALUE | _0000_SAU_CLOCK_SELECT_CK00 | _0000_SAU_TRIGGER_SOFTWARE |
            _0002_SAU_MODE_UART | _0000_SAU_TRANSFER_END;
    SCR02 = _8000_SAU_TRANSMISSION | _0000_SAU_INTSRE_MASK | _0000_SAU_PARITY_NONE | _0080_SAU_LSB | _0010_SAU_STOP_1 |
            _0007_SAU_LENGTH_8;
    SDR02 = _CE00_UART1_TRANSMIT_DIVISOR;
    NFEN0 |= _04_SAU_RXD1_FILTER_ON;
    SIR03 = _0004_SAU_SIRMN_FECTMN | _0002_SAU_SIRMN_PECTMN | _0001_SAU_SIRMN_OVCTMN;    /* clear error flag */
    SMR03 = _0020_SAU_SMRMN_INITIALVALUE | _0000_SAU_CLOCK_SELECT_CK00 | _0100_SAU_TRIGGER_RXD | _0000_SAU_EDGE_FALL |
            _0002_SAU_MODE_UART | _0000_SAU_TRANSFER_END;
    SCR03 = _4000_SAU_RECEPTION | _0000_SAU_INTSRE_MASK | _0000_SAU_PARITY_NONE | _0080_SAU_LSB | _0010_SAU_STOP_1 |
            _0007_SAU_LENGTH_8;
    SDR03 = _CE00_UART1_RECEIVE_DIVISOR;
    SO0 |= _0004_SAU_CH2_DATA_OUTPUT_1;
    SOL0 |= _0000_SAU_CHANNEL2_NORMAL;    /* output level normal */
    SOE0 |= _0004_SAU_CH2_OUTPUT_ENABLE;    /* enable UART1 output */
    /* Set RxD1 pin */
    PMC0 &= 0xF7U;
    PM0 |= 0x08U;
    /* Set TxD1 pin */
    PMC0 &= 0xFBU;
    P0 |= 0x04U;
    PM0 &= 0xFBU;
}

/***********************************************************************************************************************
* Function Name: R_UART1_Start
* Description  : This function starts the UART1 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_UART1_Start(void)
{
    SO0 |= _0004_SAU_CH2_DATA_OUTPUT_1;    /* output level normal */
    SOE0 |= _0004_SAU_CH2_OUTPUT_ENABLE;    /* enable UART1 output */
    SS0 |= _0008_SAU_CH3_START_TRG_ON | _0004_SAU_CH2_START_TRG_ON;    /* enable UART1 receive and transmit */
    STIF1 = 0U;    /* clear INTST1 interrupt flag */
    SRIF1 = 0U;    /* clear INTSR1 interrupt flag */
    STMK1 = 0U;    /* enable INTST1 interrupt */
    SRMK1 = 0U;    /* enable INTSR1 interrupt */
}

/***********************************************************************************************************************
* Function Name: R_UART1_Stop
* Description  : This function stops the UART1 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_UART1_Stop(void)
{
    STMK1 = 1U;    /* disable INTST1 interrupt */
    SRMK1 = 1U;    /* disable INTSR1 interrupt */
    ST0 |= _0008_SAU_CH3_STOP_TRG_ON | _0004_SAU_CH2_STOP_TRG_ON;    /* disable UART1 receive and transmit */
    SOE0 &= ~_0004_SAU_CH2_OUTPUT_ENABLE;    /* disable UART1 output */
    STIF1 = 0U;    /* clear INTST1 interrupt flag */
    SRIF1 = 0U;    /* clear INTSR1 interrupt flag */
   
}

/***********************************************************************************************************************
* Function Name: R_UART1_Receive
* Description  : This function receives UART1 data.
* Arguments    : rx_buf -
*                    receive buffer pointer
*                rx_num -
*                    buffer size
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/
MD_STATUS R_UART1_Receive(uint8_t * const rx_buf, uint16_t rx_num)
{
    MD_STATUS status = MD_OK;

    if (rx_num < 1U)
    {
        status = MD_ARGERROR;
    }
    else
    {
        g_uart1_rx_count = 0U;
        g_uart1_rx_length = rx_num;
        gp_uart1_rx_address = rx_buf;
    }

    return (status);
}

/***********************************************************************************************************************
* Function Name: R_UART1_Send
* Description  : This function sends UART1 data.
* Arguments    : tx_buf -
*                    transfer buffer pointer
*                tx_num -
*                    buffer size
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/
MD_STATUS R_UART1_Send(uint8_t * const tx_buf, uint16_t tx_num)
{
    MD_STATUS status = MD_OK;

    if (tx_num < 1U)
    {
        status = MD_ARGERROR;
    }
    else
    {
        gp_uart1_tx_address = tx_buf;
        g_uart1_tx_count = tx_num;
        STMK1 = 1U;    /* disable INTST1 interrupt */
        TXD1 = *gp_uart1_tx_address;
        gp_uart1_tx_address++;
        g_uart1_tx_count--;
        STMK1 = 0U;    /* enable INTST1 interrupt */
    }

    return (status);
}

/***********************************************************************************************************************
* Function Name: R_SAU1_Create
* Description  : This function initializes the SAU1 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_SAU1_Create(void)
{
    SAU1EN = 1U;    /* supply SAU1 clock */
    NOP();
    NOP();
    NOP();
    NOP();
    SPS1 = _0000_SAU_CK00_FCLK_0 | _0010_SAU_CK01_FCLK_1;
    R_CSI20_Create();
    R_IIC21_Create();
}

/***********************************************************************************************************************
* Function Name: R_CSI20_Create
* Description  : This function initializes the CSI20 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CSI20_Create(void)
{
    ST1 |= _0001_SAU_CH0_STOP_TRG_ON;    /* disable CSI20 */
    CSIMK20 = 1U;    /* disable INTCSI20 interrupt */
    CSIIF20 = 0U;    /* clear INTCSI20 interrupt flag */
    /* Set INTCSI20 low priority */
    CSIPR120 = 1U;
    CSIPR020 = 1U;
    SIR10 = _0002_SAU_SIRMN_PECTMN | _0001_SAU_SIRMN_OVCTMN;    /* clear error flag */
    SMR10 = _0020_SAU_SMRMN_INITIALVALUE | _0000_SAU_CLOCK_SELECT_CK00 | _0000_SAU_CLOCK_MODE_CKS |
            _0000_SAU_TRIGGER_SOFTWARE | _0000_SAU_MODE_CSI | _0000_SAU_TRANSFER_END;
    SCR10 = _C000_SAU_RECEPTION_TRANSMISSION | _0000_SAU_TIMING_1 | _0000_SAU_MSB | _0007_SAU_LENGTH_8;
    SDR10 = _0200_CSI20_DIVISOR;
    SO1 |= _0100_SAU_CH0_CLOCK_OUTPUT_1;    /* CSI20 clock initial level */
    SO1 &= ~_0001_SAU_CH0_DATA_OUTPUT_1;    /* CSI20 SO initial level */
    SOE1 |= _0001_SAU_CH0_OUTPUT_ENABLE;    /* enable CSI20 output */
    /* Set SI20 pin */
    PM1 |= 0x10U;
    /* Set SO20 pin */
    P1 |= 0x08U;
    PM1 &= 0xF7U;
    /* Set SCK20 pin */
    P1 |= 0x20U;
    PM1 &= 0xDFU;
}

/***********************************************************************************************************************
* Function Name: R_CSI20_Start
* Description  : This function starts the CSI20 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CSI20_Start(void)
{
    SO1 |= _0100_SAU_CH0_CLOCK_OUTPUT_1;    /* CSI20 clock initial level */
    SO1 &= ~_0001_SAU_CH0_DATA_OUTPUT_1;           /* CSI20 SO initial level */
    SOE1 |= _0001_SAU_CH0_OUTPUT_ENABLE;           /* enable CSI20 output */
    SS1 |= _0001_SAU_CH0_START_TRG_ON;             /* enable CSI20 */
    CSIIF20 = 0U;    /* clear INTCSI20 interrupt flag */
    CSIMK20 = 0U;    /* enable INTCSI20 */
}

/***********************************************************************************************************************
* Function Name: R_CSI20_Stop
* Description  : This function stops the CSI20 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CSI20_Stop(void)
{
    CSIMK20 = 1U;    /* disable INTCSI20 interrupt */
    ST1 |= _0001_SAU_CH0_STOP_TRG_ON;        /* disable CSI20 */
    SOE1 &= ~_0001_SAU_CH0_OUTPUT_ENABLE;    /* disable CSI20 output */
    CSIIF20 = 0U;    /* clear INTCSI20 interrupt flag */
}

/***********************************************************************************************************************
* Function Name: R_CSI20_Send_Receive
* Description  : This function sends and receives CSI20 data.
* Arguments    : tx_buf -
*                    transfer buffer pointer
*                tx_num -
*                    buffer size
*                rx_buf -
*                    receive buffer pointer
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/
MD_STATUS R_CSI20_Send_Receive(uint8_t * const tx_buf, uint16_t tx_num, uint8_t * const rx_buf)
{
    MD_STATUS status = MD_OK;

    if (tx_num < 1U)
    {
        status = MD_ARGERROR;
    }
    else
    {
        g_csi20_tx_count = tx_num;        /* send data count */
        gp_csi20_tx_address = tx_buf;     /* send buffer pointer */
        gp_csi20_rx_address = rx_buf;     /* receive buffer pointer */
        CSIMK20 = 1U;                     /* disable INTCSI20 interrupt */

        if (0U != gp_csi20_tx_address)
        {
            SIO20 = *gp_csi20_tx_address;    /* started by writing data to SDR[7:0] */
            gp_csi20_tx_address++;
        }
        else
        {
            SIO20 = 0xFFU;
        }

        g_csi20_tx_count--;
        CSIMK20 = 0U;                     /* enable INTCSI20 interrupt */
    }

    return (status);
}

/***********************************************************************************************************************
* Function Name: R_IIC21_Create
* Description  : This function initializes the IIC21 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_IIC21_Create(void)
{
    ST1 |= _0002_SAU_CH1_STOP_TRG_ON;    /* disable IIC21 */
    IICMK21 = 1U;    /* disable INTIIC21 interrupt */
    IICIF21 = 0U;    /* clear INTIIC21 interrupt flag */
    /* Set INTIIC21 low priority */
    IICPR121 = 1U;
    IICPR021 = 1U;
    SIR11 = _0004_SAU_SIRMN_FECTMN | _0002_SAU_SIRMN_PECTMN | _0001_SAU_SIRMN_OVCTMN;    /* clear error flag */
    SMR11 = _0020_SAU_SMRMN_INITIALVALUE | _8000_SAU_CLOCK_SELECT_CK01 | _0000_SAU_CLOCK_MODE_CKS |
            _0000_SAU_TRIGGER_SOFTWARE | _0000_SAU_EDGE_FALL | _0004_SAU_MODE_IIC | _0000_SAU_TRANSFER_END;
    SCR11 = _0000_SAU_TIMING_1 | _0000_SAU_INTSRE_MASK | _0000_SAU_PARITY_NONE | _0000_SAU_MSB | _0010_SAU_STOP_1 |
            _0007_SAU_LENGTH_8;
    SDR11 = _9E00_IIC21_DIVISOR;
    SO1 |= _0200_SAU_CH1_CLOCK_OUTPUT_1 | _0002_SAU_CH1_DATA_OUTPUT_1;
    /* Set SCL21, SDA21 pin */
    P7 |= 0x03U;
    PM7 &= 0xFCU;
}

/***********************************************************************************************************************
* Function Name: R_IIC21_Master_Send
* Description  : This function starts transferring data for IIC21 in master mode.
* Arguments    : adr -
*                    set address for select slave
*                tx_buf -
*                    transfer buffer pointer
*                tx_num -
*                    buffer size
* Return Value : None
***********************************************************************************************************************/
void R_IIC21_Master_Send(uint8_t adr, uint8_t * const tx_buf, uint16_t tx_num)
{
    g_iic21_master_status_flag = _00_SAU_IIC_MASTER_FLAG_CLEAR;    /* clear IIC21 master status flag */
    adr &= 0xFEU;    /* send mode */
    g_iic21_master_status_flag = _01_SAU_IIC_SEND_FLAG;            /* set master status flag */
    SCR11 &= ~_C000_SAU_RECEPTION_TRANSMISSION;
    SCR11 |= _8000_SAU_TRANSMISSION;
    /* Set paramater */
    g_iic21_tx_count = tx_num;
    gp_iic21_tx_address = tx_buf;
    /* Start condition */
    R_IIC21_StartCondition();
    IICIF21 = 0U;   /* clear INTIIC21 interrupt flag */
    IICMK21 = 0U;   /* enable INTIIC21 */
    SIO21 = adr;
}

/***********************************************************************************************************************
* Function Name: R_IIC21_Master_Receive
* Description  : This function starts receiving data for IIC21 in master mode.
* Arguments    : adr -
*                    set address for select slave
*                rx_buf -
*                    receive buffer pointer
*                rx_num -
*                    buffer size
* Return Value : None
***********************************************************************************************************************/
void R_IIC21_Master_Receive(uint8_t adr, uint8_t * const rx_buf, uint16_t rx_num)
{
    g_iic21_master_status_flag = _00_SAU_IIC_MASTER_FLAG_CLEAR;    /* clear master status flag */
    adr |= 0x01U;    /* receive mode */
    g_iic21_master_status_flag = _02_SAU_IIC_RECEIVE_FLAG;         /* set master status flag */
    SCR11 &= ~_C000_SAU_RECEPTION_TRANSMISSION;
    SCR11 |= _8000_SAU_TRANSMISSION;
    /* Set parameter */
    g_iic21_rx_length = rx_num;
    g_iic21_rx_count = 0U;
    gp_iic21_rx_address = rx_buf;
    /* Start condition */
    R_IIC21_StartCondition();
    IICIF21 = 0U;   /* clear INTIIC21 interrupt flag */
    IICMK21 = 0U;   /* enable INTIIC21 */
    SIO21 = adr;
}

/***********************************************************************************************************************
* Function Name: R_IIC21_Stop
* Description  : This function stops the IIC21 operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_IIC21_Stop(void)
{
    /* Stop transfer */
    IICMK21 = 1U;    /* disable INTIIC21 */
    ST1 |= _0002_SAU_CH1_STOP_TRG_ON;    /* disable IIC21 */
    IICIF21 = 0U;    /* clear INTIIC21 interrupt flag */
}

/***********************************************************************************************************************
* Function Name: R_IIC21_StartCondition
* Description  : This function starts IIC21 condition.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_IIC21_StartCondition(void)
{
    volatile uint8_t w_count;
    
    SO1 &= ~_0002_SAU_CH1_DATA_OUTPUT_1;    /* clear IIC21 SDA */
    
    /* Change the waiting time according to the system */
    for (w_count = 0U; w_count <= IIC21_WAITTIME; w_count++)
    {
        NOP();
    }
    
    SO1 &= ~_0200_SAU_CH1_CLOCK_OUTPUT_1;    /* clear IIC21 SCL */
    SOE1 |= _0002_SAU_CH1_OUTPUT_ENABLE;            /* enable IIC21 output */
    SS1 |= _0002_SAU_CH1_START_TRG_ON;              /* enable IIC21 */
}

/***********************************************************************************************************************
* Function Name: R_IIC21_StopCondition
* Description  : This function stops IIC21 condition.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_IIC21_StopCondition(void)
{
    volatile uint8_t w_count;
    
    ST1 |= _0002_SAU_CH1_STOP_TRG_ON;           /* disable IIC21 */
    SOE1 &= ~_0002_SAU_CH1_OUTPUT_ENABLE;       /* disable IIC21 output */
    SO1 &= ~_0002_SAU_CH1_DATA_OUTPUT_1;        /* clear IIC21 SDA */
    SO1 |= _0200_SAU_CH1_CLOCK_OUTPUT_1; /* set IIC21 SCL */
    
    /* Change the waiting time according to the system */
    for (w_count = 0U; w_count <= IIC21_WAITTIME; w_count++)
    {
        NOP();
    }
    
    SO1 |= _0002_SAU_CH1_DATA_OUTPUT_1;         /* set IIC21 SDA */
}

/***********************************************************************************************************************
* Function Name: R_IICA0_Create
* Description  : This function initializes the IICA0 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_IICA0_Create(void)
{
    IICA0EN = 1U; /* supply IICA0 clock */
    IICE0 = 0U; /* disable IICA0 operation */
    IICAMK0 = 1U; /* disable INTIICA0 interrupt */
    IICAIF0 = 0U; /* clear INTIICA0 interrupt flag */
    /* Set INTIICA0 low priority */
    IICAPR10 = 1U;
    IICAPR00 = 1U; 
    /* Set SCLA0, SDAA0 pin */
    P6 &= 0xFCU;
    PM6 |= 0x03U;
    SMC0 = 1U;
    IICWL0 = _54_IICA0_IICWL_VALUE;
    IICWH0 = _4D_IICA0_IICWH_VALUE;
    DFC0 = 1U; /* digital filter on */
    IICCTL01 |= _01_IICA_fCLK_HALF;
    SVA0 = _10_IICA0_MASTERADDRESS;
    STCEN0 = 1U;
    IICRSV0 = 1U;
    SPIE0 = 0U;
    WTIM0 = 1U;
    ACKE0 = 1U;
    IICAMK0 = 0U;
    IICE0 = 1U;
    LREL0 = 1U;
    /* Set SCLA0, SDAA0 pin */
    PM6 &= 0xFCU;
}

/***********************************************************************************************************************
* Function Name: R_IICA0_Stop
* Description  : This function stops IICA0 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_IICA0_Stop(void)
{
    IICE0 = 0U;    /* disable IICA0 operation */
}

/***********************************************************************************************************************
* Function Name: R_IICA0_StopCondition
* Description  : This function sets IICA0 stop condition flag.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_IICA0_StopCondition(void)
{
    SPT0 = 1U;     /* set stop condition flag */
}

/***********************************************************************************************************************
* Function Name: R_IICA0_Master_Send
* Description  : This function starts to send data as master mode.
* Arguments    : adr -
*                    transfer address
*                tx_buf -
*                    transfer buffer pointer
*                tx_num -
*                    buffer size
*                wait -
*                    wait for start condition
* Return Value : status -
*                    MD_OK or MD_ERROR1 or MD_ERROR2
***********************************************************************************************************************/
MD_STATUS R_IICA0_Master_Send(uint8_t adr, uint8_t * const tx_buf, uint16_t tx_num, uint8_t wait)
{
    MD_STATUS status = MD_OK;

    IICAMK0 = 1U;  /* disable INTIICA0 interrupt */
    
    if (1U == IICBSY0)
    {
        /* Check bus busy */
        IICAMK0 = 0U;  /* enable INTIICA0 interrupt */
        status = MD_ERROR1;
    }
    else
    {
        STT0 = 1U; /* send IICA0 start condition */
        IICAMK0 = 0U;  /* enable INTIICA0 interrupt */
        
        /* Wait */
        while (wait--)
        {
            ;
        }
        
        if (0U == STD0)
        {
            status = MD_ERROR2;
        }
		
        /* Set parameter */
        g_iica0_tx_cnt = tx_num;
        gp_iica0_tx_address = tx_buf;
        g_iica0_master_status_flag = _00_IICA_MASTER_FLAG_CLEAR;
        adr &= (uint8_t)~0x01U; /* set send mode */
        IICA0 = adr; /* send address */
    }

    return (status);
}

/***********************************************************************************************************************
* Function Name: R_IICA0_Master_Receive
* Description  : This function starts to receive IICA0 data as master mode.
* Arguments    : adr -
*                    receive address
*                rx_buf -
*                    receive buffer pointer
*                rx_num -
*                    buffer size
*                wait -
*                    wait for start condition
* Return Value : status -
*                    MD_OK or MD_ERROR1 or MD_ERROR2
***********************************************************************************************************************/
MD_STATUS R_IICA0_Master_Receive(uint8_t adr, uint8_t * const rx_buf, uint16_t rx_num, uint8_t wait)
{
    MD_STATUS status = MD_OK;

    IICAMK0 = 1U;  /* disable INTIIA0 interrupt */
    
    if (1U == IICBSY0)
    {
        /* Check bus busy */
        IICAMK0 = 0U;  /* enable INTIIA0 interrupt */
        status = MD_ERROR1;
    }
    else
    {
        STT0 = 1U; /* set IICA0 start condition */
        IICAMK0 = 0U;  /* enable INTIIA0 interrupt */
        
        /* Wait */
        while (wait--)
        {
            ;
        }
        
        if (0U == STD0)
        {
            status = MD_ERROR2;
        }
		
        /* Set parameter */
        g_iica0_rx_len = rx_num;
        g_iica0_rx_cnt = 0U;
        gp_iica0_rx_address = rx_buf;
        g_iica0_master_status_flag  = _00_IICA_MASTER_FLAG_CLEAR;
        adr |= 0x01U; /* set receive mode */
        IICA0 = adr; /* receive address */
    }

    return (status);
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
