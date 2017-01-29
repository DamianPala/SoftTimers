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
#ifndef TIMERS_H_
#define TIMERS_H_

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
#define TMS_SYSTEM_TICK_ISR_CLK     1000000   ///< System Tick ISR Clock in Hz
#define TMS_TIMERS_CLK              1000      ///< Timers Clock in Hz
/**@}*/

/*------------------------------ EXPORTED DEFINE MACROS --------------------------------*/

/*======================================================================================*/
/*                     ####### EXPORTED TYPE DECLARATIONS #######                       */
/*======================================================================================*/
/*-------------------------------- OTHER TYPEDEFS --------------------------------------*/
typedef uint32_t TMS_timeoutMS;   ///< time in ms
typedef uint32_t TMS_ticks;       ///< timer ticks

/*------------------------------------- ENUMS ------------------------------------------*/
/** @enum TMS_TimerID_T
 *        Timers ID enumerator. For define new timer, add new ID in this enumerator.
 */
/* ---------------------------- ADD NEW TIMERS HERE!!! -------------------------------*/
typedef enum TMS_TimerID_Tag
{
  SAMPLE_TIMER = 0,            ///<
  TMS_NUM_OF_TIMERS         ///< ID for count number of timer
} TMS_TimerID_T;

/** @enum TMS_TimerRet_T
 *        Timer return type enumerator for #TMS_StartTimer.
 */
typedef enum TMS_TimerRet_Tag
{
  TMS_TIMER_STARTED = 0,    ///< Timer was started successfully
  TMS_TIMER_IN_USE,         ///< Timer is already in use
} TMS_TimerRet_T;

/** @enum TMS_TimerType_T
 *        Timer type enumerator.
 */
typedef enum TMS_TimerType_Tag
{
  TMS_ONE_SHOT = 0,         ///< This timer type expiring only one time
  TMS_AUTO_RELOAD,          ///< This timer type auto reloads after expiration
} TMS_TimerType_T;

/** @enum TMS_TimerStatus_T
 *        Timers status expiration enumerator.
 */
typedef enum TMS_TimerStatus_Tag
{
  TMS_NOT_EXPIRED = false,         ///< Timer is not expired
  TMS_EXPIRED     = true           ///< Timer is expired
} TMS_TimerStatus_T;

/*------------------------------------ STRUCT ------------------------------------------*/
/** @struct TMS_Timer_T
 *          Timer structure.
 */
typedef struct TMS_Timer_Tag
{
  TMS_TimerType_T timerType;          ///< Timer type
  volatile TMS_ticks ticks;           ///< Timer ticks
  TMS_timeoutMS timeout;              ///< Timer timeout
  volatile bool expiredFlag;          ///< Timer expired flag - used for expiration indication
  void (*onExpire)(void);             ///< Pointer to function called on timer expiration event
} TMS_Timer_T;

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
void TMS_Init (void);

/**
 * @brief Function for handling timers.
 *
 *        This function contain timers handlers code. It have to be called in System tick ISR.
 *
 * @return void
 */
void TMS_TimersHandler (void);

/**
 * @brief Function for processing timers events.
 *
 *        This function must be invoked from main while loop.
 *
 * @return void
 */
void TMS_TimersEventsHandler(void);

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
TMS_TimerRet_T TMS_StartTimer (TMS_TimerType_T timerType, TMS_TimerID_T timerID, void (*onExpire)(void), TMS_timeoutMS timeout);

/**
 * @brief Function for stopping timer.
 *
 *        This function stops timer of given ID.
 *
 * @param [in] timerID of started timer.
 *
 * @return void
 */
void TMS_StopTimer (TMS_TimerID_T timerID);

/**
 * @brief Function for restarting timer.
 *
 *        This function restarts timer of given ID.
 *
 * @param [in] timerID of started timer.
 *
 * @return void
 */
void TMS_RestartTimer (TMS_TimerID_T timerID);

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
TMS_TimerStatus_T TMS_GetTimerStatus (TMS_TimerID_T timerID);

/*======================================================================================*/
/*         ####### EXPORTED INLINE FUNCTIONS AND FUNCTION-LIKE MACROS #######           */
/*======================================================================================*/

/**
 * @}
 */

#endif /* TIMERS_H_ */
