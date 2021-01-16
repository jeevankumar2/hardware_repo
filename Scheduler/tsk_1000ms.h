/** \file tsk_1000ms.h
 * \brief This is the header file for the definition of the 1000 millisecond task.
 */
#ifndef TSK_1000MS_H_
#define TSK_1000MS_H_
/******************************************************************************
* Includes
*******************************************************************************/
#include "r_cg_macrodriver.h"
/******************************************************************************
* Constants
*******************************************************************************/

/******************************************************************************
* Macros
*******************************************************************************/

/******************************************************************************
* Configuration
*******************************************************************************/

/******************************************************************************
* Typedefs
*******************************************************************************/

/******************************************************************************
* Variable Declarations
*******************************************************************************/

/******************************************************************************
* Function Prototypes
*******************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif

void Tsk_1000ms(void);
extern uint8_t u8GPSPack_data_Int;

#ifdef __cplusplus
} // extern "C"
#endif

#endif /*TSK_1000MS_H_*/

/*** End of File **************************************************************/