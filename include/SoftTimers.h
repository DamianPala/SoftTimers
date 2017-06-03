/*=======================================================================================*
 * @file    SoftTimers.h
 * @author  Damian Pala
 * @date    03-12-2016
 * @brief   Header file for Soft Timers module
 *
 *          This file contains API of Soft Timers module
 *======================================================================================*/
/*----------------------- DEFINE TO PREVENT RECURSIVE INCLUSION ------------------------*/
#ifndef SOFTTIMERS_H_
#define SOFTTIMERS_H_

/**
 * @defgroup SFTM Soft Timers Timers
 * @{
 * @brief Module for creation software timers.
 */

/*======================================================================================*/
/*                       ####### PREPROCESSOR DIRECTIVES #######                        */
/*======================================================================================*/
/*---------------------- INCLUDE DIRECTIVES FOR STANDARD HEADERS -----------------------*/
#include <stdio.h>
#include <stdbool.h>

/*----------------------- INCLUDE DIRECTIVES FOR OTHER HEADERS -------------------------*/
#include "cmsis_device.h"

/*----------------------------- LOCAL OBJECT-LIKE MACROS -------------------------------*/

/*---------------------------- LOCAL FUNCTION-LIKE MACROS ------------------------------*/

/*======================================================================================*/
/*                     ####### EXPORTED TYPE DECLARATIONS #######                       */
/*======================================================================================*/
/*---------------------------- ALL TYPE DECLARATIONS -----------------------------------*/
typedef enum SFTM_TimerRet_Tag SFTM_TimerRet_T;
typedef enum SFTM_TimerType_Tag SFTM_TimerType_T;
typedef enum SFTM_TimerStatus_Tag SFTM_TimerStatus_T;
typedef struct SFTM_Timer_Tag SFTM_Timer_T;
typedef void (*SFTM_TimerCallback)(void* pContext);     ///< timer callback on expire event
typedef uint32_t SFTM_timeoutMS;                        ///< time in ms
typedef uint32_t SFTM_ticks;                            ///< timer ticks
typedef SFTM_Timer_T* SFTM_TimerHandle_T;               ///< timer handle

/*------------------------------------- ENUMS ------------------------------------------*/
/** @enum SFTM_TimerRet_T
 *        Timer return type enumerator for #SFTM_StartTimer.
 */
enum SFTM_TimerRet_Tag
{
  SFTM_TIMER_STARTED = 0,    ///< Timer was started successfully
  SFTM_TIMER_IN_USE,         ///< Timer is already in use
};

/** @enum SFTM_TimerType_T
 *        Timer type enumerator.
 */
enum SFTM_TimerType_Tag
{
  SFTM_ONE_SHOT = 0,         ///< This timer type expiring only one time
  SFTM_AUTO_RELOAD,          ///< This timer type auto reloads after expiration
};

/** @enum SFTM_TimerStatus_T
 *        Timers status expiration enumerator.
 */
enum SFTM_TimerStatus_Tag
{
  SFTM_NOT_EXPIRED = false,         ///< Timer is not expired
  SFTM_EXPIRED     = true           ///< Timer is expired
};

/*------------------------------- STRUCT AND UNIONS ------------------------------------*/
/** @struct SFTM_Timer_T
 *          Timer structure.
 */
struct SFTM_Timer_Tag
{
  SFTM_TimerType_T timerType;           ///< Timer type
  volatile SFTM_ticks ticks;            ///< Timer ticks
  SFTM_timeoutMS timeout;               ///< Timer timeout
  volatile bool expiredFlag;            ///< Timer expired flag - used for expiration indication
  SFTM_TimerCallback onExpire;          ///< Pointer to function called on timer expiration event
  void *pContext;                       ///< Pointer to context passed to callback function on expiration event
};

/*======================================================================================*/
/*                    ####### EXPORTED OBJECT DECLARATIONS #######                      */
/*======================================================================================*/

/*======================================================================================*/
/*                   ####### EXPORTED FUNCTIONS PROTOTYPES #######                      */
/*======================================================================================*/
/**
 * @brief Function for Timers initialization.
 *
 *        This function initializes timers.
 *
 * @return void
 */
void SFTM_Init(void);


/**
 * @brief Function for handling timers.
 *
 *        This function contain timers handlers code. It have to be called in System tick ISR.
 *
 * @return void
 */
void SFTM_TimersHandler(void);


/**
 * @brief Function for processing timers events.
 *
 *        This function must be invoked from main while loop.
 *
 * @return void
 */
void SFTM_TimersEventsHandler(void);


/**
 * @brief Function for create timers.
 *
 *        This function creates given timer.
 *
 * @return SFTM_TimerHandle_T - handle of created timer.
 */
SFTM_TimerHandle_T SFTM_CreateTimer(void);


/**
 * @brief Function for starting timers.
 *
 *        This function starts given timer.
 *
 * @param [in] timerHandle of started timer.
 * @param [in] timerType of started timer.
 * @param [in] onExpire is a pointer for function called when timer expires.
 * @param [in] timeout is a time of timer period.
 *
 * @return void
 */
SFTM_TimerRet_T SFTM_StartTimer(SFTM_TimerHandle_T timerHandle, SFTM_TimerType_T timerType, SFTM_TimerCallback onExpire, void* pContext, SFTM_timeoutMS timeout);


/**
 * @brief Function for stopping timer.
 *
 *        This function stops timer of given ID.
 *
 * @param [in] timerHandle of started timer.
 *
 * @return void
 */
void SFTM_StopTimer(SFTM_TimerHandle_T timerHandle);


/**
 * @brief Function for restarting timer.
 *
 *        This function restarts timer of given ID.
 *
 * @param [in] timerHandle of started timer.
 *
 * @return void
 */
void SFTM_RestartTimer(SFTM_TimerHandle_T timerHandle);


/**
 * @brief Function for getting timer status.
 *
 *        This function gets status of certain timer by given ID.
 *
 * @param [in] timerHandle of started timer.
 *
 * @retval true if expired
 * @retval false if not expired
 */
SFTM_TimerStatus_T SFTM_GetTimerStatus(SFTM_TimerHandle_T timerHandle);


/**
 * @brief Function to getting timer tick.
 *
 *        This function gets timer tick number.
 *
 * @param [in] timerHandle of started timer.
 *
 * @return timer tick number.
 */
uint32_t SFTM_GetTimerTick(SFTM_TimerHandle_T timerHandle);


/**
 * @brief Function for getting current timers number in system.
 *
 * @return number of timers
 */
uint8_t SFTM_GetCurrentTimersNumberInSystem(void);


/**
 * @brief Function for getting maximal timers number in system.
 *
 * @return number of timers
 */
uint8_t SFTM_MaxTimersNumberInSystem(void);


/**
 * @brief Function for make hard fault.
 *
 *        If your have your own hard fault implementation, call it in this function rather than current implementation.
 *
 * @return void
 */
static inline void SFTM_ExecuteHardFault(void)
{
  void (*Fault)(void) = NULL;
  Fault();
}

/**
 * @}
 */

#endif /* SOFTTIMERS_H_ */
