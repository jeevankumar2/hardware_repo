/** \file sys_tick.c
 * \brief This module contains the system tick.
*******************************************************************************/
#include "sys_tick.h"			// For timer initialization
#include "r_cg_macrodriver.h"
/******************************************************************************
* Constants
*******************************************************************************/
/**
 *  Defines a pointer to the SysTick Reload Value Register on the microcontroller.
 */
//uint32_t volatile * const systload = (uint32_t*)&SYST_RVR;

/**
 *  Defines a pointer to the SysTick Current Value Register on the microcontroller.
 */
//uint32_t volatile * const systval = (uint32_t*)&SYST_CVR;

/**
 *  Defines a pointer to the system tick control register on the microcontroller.
 */
//uint32_t volatile * const systcon = (uint32_t*)&SYST_CSR;

/******************************************************************************
* Configuration
*******************************************************************************/

/******************************************************************************
* Macros
*******************************************************************************/

/******************************************************************************
* Typedefs
*******************************************************************************/

/******************************************************************************
* Variables
*******************************************************************************/
/**
 * System tick variable
 */
static volatile uint32_t tick = 0;

/******************************************************************************
* Function Prototypes
*******************************************************************************/

/******************************************************************************
* Functions
*******************************************************************************/
/******************************************************************************
* Function : SysTick__Init()
*//**
* \section Description Description:
*
*  This function initializes the system tick module.
*
* \param     Interval - uint32_t, system tick interval in us.
*
* \return     None.
*
* \section Example Example:
* \code
*  	void Tmr_SysTickInit(uint32_t Interval)
*	{
*		SysTick_Init(Interval);
*	}
* \endcode
*
* \see
*
*  ----------------------
*  - HISTORY OF CHANGES -
*  ----------------------
*    Date    Software Version    Initials   Description
*  01/02/14       0.1.1            JWB      Function Created.
*
*******************************************************************************/
#if 0
void SysTick_Init(uint32_t Interval)
{
	// Set the system tick reload value with the desired interval
	*systload = (GetSystemClock() / (1000000UL / Interval)) - 1;

	*systval = 0;		// Clear the current counter value

	*systcon = 0;									// Clear the system tick control and status register
	*systcon |= (REGBIT2 + REGBIT1 + REGBIT0);		// Enable the system tick with interrupt
}
#endif

/******************************************************************************
* Function : SysTick_GetSystemTick()
*//**
* \section Description Description:
*
*  This function returns the current system tick value
*
* \param     None.
*
* \return     uint32_t - system tick
*
* \section Example Example:
* \code
*	uint32_t tick = SysTick_GetSystemTick();
* \endcode
*
* \see
*
*  ----------------------
*  - HISTORY OF CHANGES -
*  ----------------------
*    Date    Software Version    Initials   Description
*  01/02/14       0.1.1            JWB      Function Created.
*
*******************************************************************************/
uint32_t SysTick_GetSystemTick()
{
	return tick;
}

/******************************************************************************
* Function : SysTick_ISR()
*//**
* \section Description Description:
*
*  This function is the System Tick Interrupt Serive Routine.  Increment the
*  system tick.
*
* \param     None.
*
* \return     None.
*
* \see
*
*  ----------------------
*  - HISTORY OF CHANGES -
*  ----------------------
*    Date    Software Version    Initials   Description
*  01/02/14       0.1.1                 	Function Created.
*
*******************************************************************************/
void SysTick_ISR(void)
{
	tick++;
}

/*** End of File **************************************************************/
