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
#define SFTM_SYSTEM_TICK_ISR_CLK     1000000   ///< System Tick ISR Clock in Hz
#define SFTM_TIMERS_CLK              1000      ///< Timers Clock in Hz
/**@}*/

/*------------------------------ EXPORTED DEFINE MACROS --------------------------------*/

/*======================================================================================*/
/*                     ####### EXPORTED TYPE DECLARATIONS #######                       */
/*======================================================================================*/
/*-------------------------------- OTHER TYPEDEFS --------------------------------------*/
typedef uint32_t SFTM_timeoutMS;   ///< time in ms
typedef uint32_t SFTM_ticks;       ///< timer ticks

/*------------------------------------- ENUMS ------------------------------------------*/
/** @enum SFTM_TimerID_T
 *        Timers ID enumerator. For define new timer, add new ID in this enumerator.
 */
/* ---------------------------- ADD NEW TIMERS HERE!!! -------------------------------*/
typedef enum SFTM_TimerID_Tag
{
  SAMPLE_TIMER = 0,            ///<
  SFTM_NUM_OF_TIMERS         ///< ID for count number of timer
} SFTM_TimerID_T;

/** @enum SFTM_TimerRet_T
 *        Timer return type enumerator for #SFTM_StartTimer.
 */
typedef enum SFTM_TimerRet_Tag
{
  SFTM_TIMER_STARTED = 0,    ///< Timer was started successfully
  SFTM_TIMER_IN_USE,         ///< Timer is already in use
} SFTM_TimerRet_T;

/** @enum SFTM_TimerType_T
 *        Timer type enumerator.
 */
typedef enum SFTM_TimerType_Tag
{
  SFTM_ONE_SHOT = 0,         ///< This timer type expiring only one time
  SFTM_AUTO_RELOAD,          ///< This timer type auto reloads after expiration
} SFTM_TimerType_T;

/** @enum SFTM_TimerStatus_T
 *        Timers status expiration enumerator.
 */
typedef enum SFTM_TimerStatus_Tag
{
  SFTM_NOT_EXPIRED = false,         ///< Timer is not expired
  SFTM_EXPIRED     = true           ///< Timer is expired
} SFTM_TimerStatus_T;

/*------------------------------------ STRUCT ------------------------------------------*/
/** @struct SFTM_Timer_T
 *          Timer structure.
 */
typedef struct SFTM_Timer_Tag
{
  SFTM_TimerType_T timerType;          ///< Timer type
  volatile SFTM_ticks ticks;           ///< Timer ticks
  SFTM_timeoutMS timeout;              ///< Timer timeout
  volatile bool expiredFlag;          ///< Timer expired flag - used for expiration indication
  void (*onExpire)(void);             ///< Pointer to function called on timer expiration event
} SFTM_Timer_T;

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
void SFTM_Init (void);

/**
 * @brief Function for handling timers.
 *
 *        This function contain timers handlers code. It have to be called in System tick ISR.
 *
 * @return void
 */
void SFTM_TimersHandler (void);

/**
 * @brief Function for processing timers events.
 *
 *        This function must be invoked from main while loop.
 *
 * @return void
 */
void SFTM_TimersEventsHandler(void);

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
SFTM_TimerRet_T SFTM_StartTimer (SFTM_TimerType_T timerType, SFTM_TimerID_T timerID, void (*onExpire)(void), SFTM_timeoutMS timeout);

/**
 * @brief Function for stopping timer.
 *
 *        This function stops timer of given ID.
 *
 * @param [in] timerID of started timer.
 *
 * @return void
 */
void SFTM_StopTimer (SFTM_TimerID_T timerID);

/**
 * @brief Function for restarting timer.
 *
 *        This function restarts timer of given ID.
 *
 * @param [in] timerID of started timer.
 *
 * @return void
 */
void SFTM_RestartTimer (SFTM_TimerID_T timerID);

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
SFTM_TimerStatus_T SFTM_GetTimerStatus (SFTM_TimerID_T timerID);

/*======================================================================================*/
/*         ####### EXPORTED INLINE FUNCTIONS AND FUNCTION-LIKE MACROS #######           */
/*======================================================================================*/

/**
 * @}
 */

#endif /* SOFTTIMERS_H_ */
