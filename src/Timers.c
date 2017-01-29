/*=======================================================================================*
 * @file    Timers.c
 * @author  Damian Pala
 * @version 0.1
 * @date    03-12-2016
 * @brief   This file contains all implementations for Timers module.
 *======================================================================================*/

/**
 * @addtogroup TMS Timers
 * @{
 * @brief Module for creation software timers.
 */

/*======================================================================================*/
/*                       ####### PREPROCESSOR DIRECTIVES #######                        */
/*======================================================================================*/
/*---------------------- INCLUDE DIRECTIVES FOR STANDARD HEADERS -----------------------*/

/*----------------------- INCLUDE DIRECTIVES FOR OTHER HEADERS -------------------------*/
#include "Timers.h"

/*--------------------------- LOCAL DEFINES FOR CONSTANTS ------------------------------*/
#define TICK_CMP              TMS_SYSTEM_TICK_ISR_CLK / TMS_TIMERS_CLK ///< Comparison value for timers handler
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
static TMS_Timer_T TMS_TimersArray[TMS_NUM_OF_TIMERS]; // Timers array

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
void TMS_Init (void)
{
  for (uint8_t timerID = 0; timerID < TMS_NUM_OF_TIMERS; timerID++)
  {
    TMS_TimersArray[timerID].ticks        = TIMIER_INIT_VALUE;
    TMS_TimersArray[timerID].timeout      = 0;
    TMS_TimersArray[timerID].expiredFlag  = false;
    TMS_TimersArray[timerID].onExpire     = NULL;
  }
}

void TMS_TimersHandler (void)
{
  static volatile uint32_t ticks = 0;

  ticks++;

  if (TICK_CMP == ticks)
  {
    /* Clear base ticks */
    ticks = 0;

    for (uint8_t timerID = 0; timerID < TMS_NUM_OF_TIMERS; timerID++)
    {
      if (TMS_TimersArray[timerID].expiredFlag != true)
      {
        /* Increment timer ticks */
        TMS_TimersArray[timerID].ticks++;

        /* Check if expires */
        if (TMS_TimersArray[timerID].ticks == TMS_TimersArray[timerID].timeout)
        {
          TMS_TimersArray[timerID].expiredFlag = true;
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

TMS_TimerRet_T TMS_StartTimer (TMS_TimerType_T timerType, TMS_TimerID_T timerID, void (*onExpire)(void), TMS_timeoutMS timeout)
{
  TMS_TimerRet_T ret;
  if (TMS_TimersArray[timerID].ticks <= TMS_TimersArray[timerID].timeout)
  {
    ret = TMS_TIMER_IN_USE;
  }
  else
  {
    TMS_TimersArray[timerID].timerType    = timerType;
    TMS_TimersArray[timerID].timeout      = timeout;
    TMS_TimersArray[timerID].onExpire     = onExpire;
    TMS_TimersArray[timerID].ticks        = 0;
    TMS_TimersArray[timerID].expiredFlag  = false;

    ret = TMS_TIMER_STARTED;
  }

  return ret;
}

void TMS_StopTimer (TMS_TimerID_T timerID)
{
  TMS_TimersArray[timerID].ticks        = TIMIER_INIT_VALUE;
  TMS_TimersArray[timerID].timeout      = 0;
  TMS_TimersArray[timerID].expiredFlag  = false;
  TMS_TimersArray[timerID].onExpire     = NULL;
}

void TMS_RestartTimer (TMS_TimerID_T timerID)
{
  TMS_TimersArray[timerID].ticks        = 0;
  TMS_TimersArray[timerID].expiredFlag  = false;
}

void TMS_TimersEventsHandler(void)
{
  for (uint8_t timerID = 0; timerID < TMS_NUM_OF_TIMERS; timerID++)
  {
    if (true == TMS_TimersArray[timerID].expiredFlag)
    {
      /* Call timer event if is not NULL */
      if (TMS_TimersArray[timerID].onExpire != NULL)
      {
        TMS_TimersArray[timerID].onExpire();
      }
      else { /* Do nothing */ }

      if (TMS_ONE_SHOT == TMS_TimersArray[timerID].timerType)
      {
        /* No more calls onExpire function */
        TMS_TimersArray[timerID].expiredFlag = false;
      }
      else // TMS_AUTO_RELOAD
      {
        TMS_RestartTimer((TMS_TimerID_T)timerID);
      }
    }
    else
    {
      /* Do nothing */
    }
  }
}

TMS_TimerStatus_T TMS_GetTimerStatus (TMS_TimerID_T timerID)
{
  if (TMS_TimersArray[timerID].ticks > TMS_TimersArray[timerID].timeout)
  {
    return TMS_EXPIRED;
  }
  else
  {
    return TMS_NOT_EXPIRED;
  }
}

/**
 * @}
 */
