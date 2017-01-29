/*=======================================================================================*
 * @file    Timers.c
 * @author  Damian Pala
 * @date    03-12-2016
 * @brief   This file contains all implementations for Soft Timers module.
 *======================================================================================*/

/**
 * @addtogroup Soft Timers Timers
 * @{
 * @brief Module for creation software timers.
 */

/*======================================================================================*/
/*                       ####### PREPROCESSOR DIRECTIVES #######                        */
/*======================================================================================*/
/*---------------------- INCLUDE DIRECTIVES FOR STANDARD HEADERS -----------------------*/

/*----------------------- INCLUDE DIRECTIVES FOR OTHER HEADERS -------------------------*/
#include "SoftTimers.h"

/*--------------------------- LOCAL DEFINES FOR CONSTANTS ------------------------------*/
#define TICK_CMP              SOFTTIMERS_SYSTEM_TICK_ISR_CLK / SOFTTIMERS_TIMERS_CLK ///< Comparison value for timers handler
#define TIMIER_INIT_VALUE     0xFFFFFFFF                             ///< Initial timer value

/*------------------------------- LOCAL DEFINE MACROS ----------------------------------*/

/*======================================================================================*/
/*                      ####### LOCAL TYPE DECLARATIONS #######                         */
/*======================================================================================*/
/*-------------------------------- OTHER TYPEDEFS --------------------------------------*/

/*------------------------------------- ENUMS ------------------------------------------*/

/*------------------------------------ STRUCT ------------------------------------------*/

/*------------------------------------ UNIONS ------------------------------------------*/

/*======================================================================================*/
/*                         ####### OBJECT DEFINITIONS #######                           */
/*======================================================================================*/
/*------------------------------- EXPORTED OBJECTS -------------------------------------*/

/*-------------------------------- LOCAL OBJECTS ---------------------------------------*/
static SoftTimers_Timer_T TimersArray[SOFTTIMERS_NUM_OF_TIMERS]; // Timers array

/*======================================================================================*/
/*                    ####### LOCAL FUNCTIONS PROTOTYPES #######                        */
/*======================================================================================*/

/*======================================================================================*/
/*          ####### LOCAL INLINE FUNCTIONS AND FUNCTION-LIKE MACROS #######             */
/*======================================================================================*/

/*======================================================================================*/
/*                   ####### LOCAL FUNCTIONS DEFINITIONS #######                        */
/*======================================================================================*/

/*======================================================================================*/
/*                 ####### EXPORTED FUNCTIONS DEFINITIONS #######                       */
/*======================================================================================*/
void SoftTimers_Init (void)
{
  for (uint8_t timerID = 0; timerID < SOFTTIMERS_NUM_OF_TIMERS; timerID++)
  {
    TimersArray[timerID].ticks        = TIMIER_INIT_VALUE;
    TimersArray[timerID].timeout      = 0;
    TimersArray[timerID].expiredFlag  = false;
    TimersArray[timerID].onExpire     = NULL;
  }
}

void SoftTimers_TimersHandler (void)
{
  static volatile uint32_t ticks = 0;

  ticks++;

  if (TICK_CMP == ticks)
  {
    /* Clear base ticks */
    ticks = 0;

    for (uint8_t timerID = 0; timerID < SOFTTIMERS_NUM_OF_TIMERS; timerID++)
    {
      if (TimersArray[timerID].expiredFlag != true)
      {
        /* Increment timer ticks */
        TimersArray[timerID].ticks++;

        /* Check if expires */
        if (TimersArray[timerID].ticks == TimersArray[timerID].timeout)
        {
          TimersArray[timerID].expiredFlag = true;
        }
        else
        {
          /* Do nothing */
        }
      }
      else
      {
        /* Do nothing */
      }
    }
  }
  else
  {
    /* Do nothing */
  }
}

SoftTimers_TimerRet_T SoftTimers_StartTimer (SoftTimers_TimerType_T timerType, SoftTimers_TimerID_T timerID, void (*onExpire)(void), SoftTimers_timeoutMS timeout)
{
  SoftTimers_TimerRet_T ret;
  if (TimersArray[timerID].ticks <= TimersArray[timerID].timeout)
  {
    ret = SOFTTIMERS_TIMER_IN_USE;
  }
  else
  {
    TimersArray[timerID].timerType    = timerType;
    TimersArray[timerID].timeout      = timeout;
    TimersArray[timerID].onExpire     = onExpire;
    TimersArray[timerID].ticks        = 0;
    TimersArray[timerID].expiredFlag  = false;

    ret = SOFTTIMERS_TIMER_STARTED;
  }

  return ret;
}

void SoftTimers_StopTimer (SoftTimers_TimerID_T timerID)
{
  TimersArray[timerID].ticks        = TIMIER_INIT_VALUE;
  TimersArray[timerID].timeout      = 0;
  TimersArray[timerID].expiredFlag  = false;
  TimersArray[timerID].onExpire     = NULL;
}

void SoftTimers_RestartTimer (SoftTimers_TimerID_T timerID)
{
  TimersArray[timerID].ticks        = 0;
  TimersArray[timerID].expiredFlag  = false;
}

void SoftTimers_TimersEventsHandler(void)
{
  for (uint8_t timerID = 0; timerID < SOFTTIMERS_NUM_OF_TIMERS; timerID++)
  {
    if (true == TimersArray[timerID].expiredFlag)
    {
      /* Call timer event if is not NULL */
      if (TimersArray[timerID].onExpire != NULL)
      {
        TimersArray[timerID].onExpire();
      }
      else { /* Do nothing */ }

      if (SOFTTIMERS_ONE_SHOT == TimersArray[timerID].timerType)
      {
        /* No more calls onExpire function */
        TimersArray[timerID].expiredFlag = false;
      }
      else // SOFTTIMERS_AUTO_RELOAD
      {
        SoftTimers_RestartTimer((SoftTimers_TimerID_T)timerID);
      }
    }
    else
    {
      /* Do nothing */
    }
  }
}

SoftTimers_TimerStatus_T SoftTimers_GetTimerStatus (SoftTimers_TimerID_T timerID)
{
  if (TimersArray[timerID].ticks > TimersArray[timerID].timeout)
  {
    return SOFTTIMERS_EXPIRED;
  }
  else
  {
    return SOFTTIMERS_NOT_EXPIRED;
  }
}

/**
 * @}
 */
