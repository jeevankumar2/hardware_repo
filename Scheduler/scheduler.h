/** \file Scheduler.h
 * \brief This module contains definitions for a task scheduler.
 */
#ifndef SCHEDULER_H_
#define SCHEDULER_H_
/******************************************************************************
* Includes
*******************************************************************************/
#include "r_cg_macrodriver.h"
//#include <stdint.h>				// For standard type definitions
/******************************************************************************
* Constants
*******************************************************************************/
/**
* Define the system tick interval in microseconds
 */
#define SYS_TICK_INTERVAL			10000UL

/**
 * The interval to call the 10 ms tasks
 */
#define INTERVAL_10MS        (10000UL / SYS_TICK_INTERVAL)

/**
 * The interval to call the 50 ms tasks
 */
#define INTERVAL_50MS        (50000UL / SYS_TICK_INTERVAL)

/**
 * The interval to call the 100 ms tasks
 */
#define INTERVAL_100MS       (100000UL / SYS_TICK_INTERVAL)

/**
 * The interval to call the 200 ms tasks
 */
#define INTERVAL_200MS       (200000UL / SYS_TICK_INTERVAL)

/**
 * The interval to call the 500 ms tasks
 */
#define INTERVAL_500MS       (500000UL / SYS_TICK_INTERVAL)
//#define INTERVAL_500MS       (200000UL / SYS_TICK_INTERVAL)

/**
 * The interval to call the 1000 ms tasks
 */
#define INTERVAL_1000MS       (1000000UL / SYS_TICK_INTERVAL)

/******************************************************************************
* Macros
*******************************************************************************/

/******************************************************************************
* Configuration
*******************************************************************************/

/******************************************************************************
* Typedefs
*******************************************************************************/
/**
 * Struct TaskType
 * TaskType structure is used to define the parameters required in order to
 * configure a task.
 */
//typedef struct
//{
//  uint16_t Interval;			/**< Defines how often a task will run  */
//  uint32_t LastTick;			/**< Stores the last tick task was ran  */
//  void (*Func)(void);			/**< Function pointer to the task  */
//}TaskType;

typedef struct
{
  uint16_t Interval;
  uint32_t LastTick;
  void (*Func)(void);
}TaskType;

/******************************************************************************
* Variable Declarations
*******************************************************************************/

/******************************************************************************
* Function Prototypes
*******************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif

#ifdef __cplusplus
} // extern "C"
#endif

#endif /*SCHEDULER_H_*/
/*** End of File **************************************************************/
