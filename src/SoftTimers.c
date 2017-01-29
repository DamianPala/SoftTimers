/*=======================================================================================*
 * @file    SFTM.c
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
#define TICK_CMP              SFTM_SYSTEM_TICK_ISR_CLK / SFTM_TIMERS_CLK ///< Comparison value for timers handler
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
static SFTM_Timer_T TimersArray[SFTM_NUM_OF_TIMERS]; // Timers array

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
void SFTM_Init (void)
{
  for (uint8_t timerID = 0; timerID < SFTM_NUM_OF_TIMERS; timerID++)
  {
    TimersArray[timerID].ticks        = TIMIER_INIT_VALUE;
    TimersArray[timerID].timeout      = 0;
    TimersArray[timerID].expiredFlag  = false;
    TimersArray[timerID].onExpire     = NULL;
  }
}

void SFTM_TimersHandler (void)
{
  static volatile uint32_t ticks = 0;

  ticks++;

  if (TICK_CMP == ticks)
  {
    /* Clear base ticks */
    ticks = 0;

    for (uint8_t timerID = 0; timerID < SFTM_NUM_OF_TIMERS; timerID++)
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

SFTM_TimerRet_T SFTM_StartTimer (SFTM_TimerType_T timerType, SFTM_TimerID_T timerID, void (*onExpire)(void), SFTM_timeoutMS timeout)
{
  SFTM_TimerRet_T ret;
  if (TimersArray[timerID].ticks <= TimersArray[timerID].timeout)
  {
    ret = SFTM_TIMER_IN_USE;
  }
  else
  {
    TimersArray[timerID].timerType    = timerType;
    TimersArray[timerID].timeout      = timeout;
    TimersArray[timerID].onExpire     = onExpire;
    TimersArray[timerID].ticks        = 0;
    TimersArray[timerID].expiredFlag  = false;

    ret = SFTM_TIMER_STARTED;
  }

  return ret;
}

void SFTM_StopTimer (SFTM_TimerID_T timerID)
{
  TimersArray[timerID].ticks        = TIMIER_INIT_VALUE;
  TimersArray[timerID].timeout      = 0;
  TimersArray[timerID].expiredFlag  = false;
  TimersArray[timerID].onExpire     = NULL;
}

void SFTM_RestartTimer (SFTM_TimerID_T timerID)
{
  TimersArray[timerID].ticks        = 0;
  TimersArray[timerID].expiredFlag  = false;
}

void SFTM_TimersEventsHandler(void)
{
  for (uint8_t timerID = 0; timerID < SFTM_NUM_OF_TIMERS; timerID++)
  {
    if (true == TimersArray[timerID].expiredFlag)
    {
      /* Call timer event if is not NULL */
      if (TimersArray[timerID].onExpire != NULL)
      {
        TimersArray[timerID].onExpire();
      }
      else { /* Do nothing */ }

      if (SFTM_ONE_SHOT == TimersArray[timerID].timerType)
      {
        /* No more calls onExpire function */
        TimersArray[timerID].expiredFlag = false;
      }
      else // SFTM_AUTO_RELOAD
      {
        SFTM_RestartTimer((SFTM_TimerID_T)timerID);
      }
    }
    else
    {
      /* Do nothing */
    }
  }
}

SFTM_TimerStatus_T SFTM_GetTimerStatus (SFTM_TimerID_T timerID)
{
  if (TimersArray[timerID].ticks > TimersArray[timerID].timeout)
  {
    return SFTM_EXPIRED;
  }
  else
  {
    return SFTM_NOT_EXPIRED;
  }
}

/**
 * @}
 */
