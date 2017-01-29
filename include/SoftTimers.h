/*=======================================================================================*
 * @file    Timers.h
 * @author  Damian Pala
 * @version 0.1
 * @date    03-12-2016
 * @brief   Header file for Timers module
 *
 *          This file contains API of Timers module
 *======================================================================================*/
/*----------------------- DEFINE TO PREVENT RECURSIVE INCLUSION ------------------------*/
#ifndef SOFTTIMERS_H_
#define SOFTTIMERS_H_

/**
 * @defgroup TMS Timers
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

/*-------------------------- EXPORTED DEFINES FOR CONSTANTS ----------------------------*/
/** @name Timers module configuration.
 *        Configure System Tick ISR Clock and Timers Clock.
 */
/**@{*/
#define SOFTTIMERS_SYSTEM_TICK_ISR_CLK     1000000   ///< System Tick ISR Clock in Hz
#define SOFTTIMERS_TIMERS_CLK              1000      ///< Timers Clock in Hz
/**@}*/

/*------------------------------ EXPORTED DEFINE MACROS --------------------------------*/

/*======================================================================================*/
/*                     ####### EXPORTED TYPE DECLARATIONS #######                       */
/*======================================================================================*/
/*-------------------------------- OTHER TYPEDEFS --------------------------------------*/
typedef uint32_t SoftTimers_timeoutMS;   ///< time in ms
typedef uint32_t SoftTimers_ticks;       ///< timer ticks

/*------------------------------------- ENUMS ------------------------------------------*/
/** @enum SoftTimers_TimerID_T
 *        Timers ID enumerator. For define new timer, add new ID in this enumerator.
 */
/* ---------------------------- ADD NEW TIMERS HERE!!! -------------------------------*/
typedef enum SoftTimers_TimerID_Tag
{
  SAMPLE_TIMER = 0,            ///<
  SOFTTIMERS_NUM_OF_TIMERS         ///< ID for count number of timer
} SoftTimers_TimerID_T;

/** @enum SoftTimers_TimerRet_T
 *        Timer return type enumerator for #SoftTimers_StartTimer.
 */
typedef enum SoftTimers_TimerRet_Tag
{
  SOFTTIMERS_TIMER_STARTED = 0,    ///< Timer was started successfully
  SOFTTIMERS_TIMER_IN_USE,         ///< Timer is already in use
} SoftTimers_TimerRet_T;

/** @enum SoftTimers_TimerType_T
 *        Timer type enumerator.
 */
typedef enum SoftTimers_TimerType_Tag
{
  SOFTTIMERS_ONE_SHOT = 0,         ///< This timer type expiring only one time
  SOFTTIMERS_AUTO_RELOAD,          ///< This timer type auto reloads after expiration
} SoftTimers_TimerType_T;

/** @enum SoftTimers_TimerStatus_T
 *        Timers status expiration enumerator.
 */
typedef enum SoftTimers_TimerStatus_Tag
{
  SOFTTIMERS_NOT_EXPIRED = false,         ///< Timer is not expired
  SOFTTIMERS_EXPIRED     = true           ///< Timer is expired
} SoftTimers_TimerStatus_T;

/*------------------------------------ STRUCT ------------------------------------------*/
/** @struct SoftTimers_Timer_T
 *          Timer structure.
 */
typedef struct SoftTimers_Timer_Tag
{
  SoftTimers_TimerType_T timerType;          ///< Timer type
  volatile SoftTimers_ticks ticks;           ///< Timer ticks
  SoftTimers_timeoutMS timeout;              ///< Timer timeout
  volatile bool expiredFlag;          ///< Timer expired flag - used for expiration indication
  void (*onExpire)(void);             ///< Pointer to function called on timer expiration event
} SoftTimers_Timer_T;

/*------------------------------------ UNIONS ------------------------------------------*/

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
void SoftTimers_Init (void);

/**
 * @brief Function for handling timers.
 *
 *        This function contain timers handlers code. It have to be called in System tick ISR.
 *
 * @return void
 */
void SoftTimers_TimersHandler (void);

/**
 * @brief Function for processing timers events.
 *
 *        This function must be invoked from main while loop.
 *
 * @return void
 */
void SoftTimers_TimersEventsHandler(void);

/**
 * @brief Function for starting timers.
 *
 *        This function starts given timer.
 *
 * @param [in] timerType of started timer.
 * @param [in] timerID of started timer.
 * @param [in] onExpire is a pointer for function called when timer expires.
 * @param [in] timeout is a time of timer period.
 *
 * @return void
 */
SoftTimers_TimerRet_T SoftTimers_StartTimer (SoftTimers_TimerType_T timerType, SoftTimers_TimerID_T timerID, void (*onExpire)(void), SoftTimers_timeoutMS timeout);

/**
 * @brief Function for stopping timer.
 *
 *        This function stops timer of given ID.
 *
 * @param [in] timerID of started timer.
 *
 * @return void
 */
void SoftTimers_StopTimer (SoftTimers_TimerID_T timerID);

/**
 * @brief Function for restarting timer.
 *
 *        This function restarts timer of given ID.
 *
 * @param [in] timerID of started timer.
 *
 * @return void
 */
void SoftTimers_RestartTimer (SoftTimers_TimerID_T timerID);

/**
 * @brief Function for getting timer status.
 *
 *        This function gets status of certain timer by given ID.
 *
 * @param [in] timerID of started timer.
 *
 * @return true if expired
 * @return false if not expired
 */
SoftTimers_TimerStatus_T SoftTimers_GetTimerStatus (SoftTimers_TimerID_T timerID);

/*======================================================================================*/
/*         ####### EXPORTED INLINE FUNCTIONS AND FUNCTION-LIKE MACROS #######           */
/*======================================================================================*/

/**
 * @}
 */

#endif /* SOFTTIMERS_H_ */
