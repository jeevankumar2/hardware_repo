/******************************************************************************
* Includes
*******************************************************************************/
#include "tsk.h"                   	// For continuous tasks
#include "tsk_10ms.h"             	// For 10 ms tasks
#include "tsk_100ms.h"             	// For 100 ms tasks
#include "tsk_500ms.h"             	// For 500 ms tasks
#include "scheduler.h"				// For task interval definitions
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
/**
 * Task configuration table.  Holds the task interval, last time executed, and
 * the function to be executed.  A continuous task is defined as a task with
 * an interval of 0.  Last time executed is set to 0.
 */
 
/*static TaskType Tasks[] =
{
  	{INTERVAL_200MS,0, Tsk_200ms},
	{INTERVAL_500MS,0, Tsk_500ms},
	{INTERVAL_1000MS,0, Tsk_1000ms},
};*/


static TaskType Tasks[] =
{
	{INTERVAL_100MS,0, Tsk_100ms},
	{INTERVAL_500MS,0, Tsk_500ms},

};


/*static TaskType Tasks[] =
{
	{INTERVAL_500MS,0, Tsk_500ms},
};*/

/**
 * Defines the number of tasks that are currently scheduled to run.
 */
/**
 * Defines the number of tasks that are currently scheduled to run.
 */
uint8_t Task_Number = sizeof(Tasks) / sizeof(*Tasks);

/******************************************************************************
* Function Prototypes
*******************************************************************************/

/******************************************************************************
* Function Definitions
*******************************************************************************/
/******************************************************************************
* Function : Tsk_GetConfig()
*//**
* \section Description Description:
*  The Tsk_GetConfig function returns a pointer to the task configuration table.
*
* \param  None.
*
* \return TaskType * - Pointer to the Task configuration table
*
* \section Example Example:
* \code
*    static TaskType *Task_ptr;
*
*    Task_Ptr = Tsk_GetConfig();
* \endcode
*
* \see Tsk_GetNumTasks
*
*  ----------------------
*  - HISTORY OF CHANGES -
*  ----------------------
*    Date    Software Version    Initials   Description
*  01/02/14       0.1.1            JWB      Function Created.
*
*******************************************************************************/
TaskType *Tsk_GetConfig(void)
{
   	return Tasks;
}

/******************************************************************************
* Function : Tsk_GetNumTasks()
*//**
* \section Description Description:
*  The Tsk_GetNumTasks function returns the number of tasks that are in the
*  task configuration table.
*
* \param  None.
*
* \return uint8_t - The number of tasks in the task configuration table.
*
* \section Example Example:
* \code
*    const uint8_t NumTasks = Tsk_GetNumTasks();
* \endcode
*
* \see Tsk_GetConfig
*
*  ----------------------
*  - HISTORY OF CHANGES -
*  ----------------------
*
*******************************************************************************/
uint8_t Tsk_GetNumTasks(void)
{
	return (sizeof(Tasks) / sizeof(*Tasks));
}

/*** End of File **************************************************************/