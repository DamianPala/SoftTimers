/*=======================================================================================*
 * @file    SFTM.c
 * @author  Damian Pala
 * @date    03-12-2016
 * @brief   This file contains all implementations for Soft Timers module.
 *======================================================================================*/

/**
 * @addtogroup SFTM Soft Timers Timers
 * @{
 * @brief Module for creation software timers.
 */

/*======================================================================================*/
/*                       ####### PREPROCESSOR DIRECTIVES #######                        */
/*======================================================================================*/
/*---------------------- INCLUDE DIRECTIVES FOR STANDARD HEADERS -----------------------*/

/*----------------------- INCLUDE DIRECTIVES FOR OTHER HEADERS -------------------------*/
#include "SoftTimers.h"

/*----------------------------- LOCAL OBJECT-LIKE MACROS -------------------------------*/
/** @name Timers module configuration.
 *        Configure System Tick ISR Clock, Timers Clock and some other things.
 */
/**@{*/
#define SYSTEM_TICK_ISR_CLK           1000000    ///< System Tick ISR Clock in Hz
#define TIMERS_CLK                    1000       ///< Timers Clock in Hz
#define MAX_TIMER_SLOTS               8          ///< Adjust this value according to your needs
/**@}*/

#define TICK_CMP                      SYSTEM_TICK_ISR_CLK / TIMERS_CLK    ///< Comparison value for timers handler
#define TIMIER_IDLE_VALUE             0xFFFFFFFF                          ///< Initial timer value
#define MAX_TIMERS_NUMBER_REACHED     0xFF                                ///< Maximum number of timers in system

#if (MAX_TIMER_SLOTS > MAX_TIMERS_NUMBER_REACHED )
  #error "Maximum timer slots reached! Please decrease timer slot number."
#endif

/*---------------------------- LOCAL FUNCTION-LIKE MACROS ------------------------------*/

/*======================================================================================*/
/*                      ####### LOCAL TYPE DECLARATIONS #######                         */
/*======================================================================================*/
/*-------------------------------- OTHER TYPEDEFS --------------------------------------*/

/*------------------------------------- ENUMS ------------------------------------------*/

/*------------------------------- STRUCT AND UNIONS ------------------------------------*/

/*======================================================================================*/
/*                         ####### OBJECT DEFINITIONS #######                           */
/*======================================================================================*/
/*------------------------------- EXPORTED OBJECTS -------------------------------------*/

/*-------------------------------- LOCAL OBJECTS ---------------------------------------*/
static SFTM_Timer_T TimersArray[MAX_TIMER_SLOTS]; ///< Timers array
static uint8_t CurrentTimersNumber = 0;           ///< Variable for storing current number of timers in system

/*======================================================================================*/
/*                    ####### LOCAL FUNCTIONS PROTOTYPES #######                        */
/*======================================================================================*/

/*======================================================================================*/
/*                   ####### LOCAL FUNCTIONS DEFINITIONS #######                        */
/*======================================================================================*/

/*======================================================================================*/
/*                  ####### EXPORTED FUNCTIONS DEFINITIONS #######                      */
/*======================================================================================*/
void SFTM_Init(void)
{
  for (uint8_t timerCnt = 0; timerCnt < MAX_TIMER_SLOTS; timerCnt++)
  {
    TimersArray[timerCnt].ticks        = TIMIER_IDLE_VALUE;
    TimersArray[timerCnt].timeout      = 0;
    TimersArray[timerCnt].expiredFlag  = false;
    TimersArray[timerCnt].onExpire     = NULL;
    TimersArray[timerCnt].pContext     = NULL;
  }
}

void SFTM_TimersHandler(void)
{
  static volatile uint32_t ticks = 0;

  ticks++;

  if (TICK_CMP == ticks)
  {
    /* Clear base ticks */
    ticks = 0;

    for (uint8_t timerCnt = 0; timerCnt < MAX_TIMER_SLOTS; timerCnt++)
    {
      if (TimersArray[timerCnt].expiredFlag != true && TimersArray[timerCnt].ticks != TIMIER_IDLE_VALUE)
      {
        /* Increment timer ticks */
        TimersArray[timerCnt].ticks++;

        /* Check if expires */
        if (TimersArray[timerCnt].ticks == TimersArray[timerCnt].timeout)
        {
          TimersArray[timerCnt].expiredFlag = true;
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

SFTM_TimerHandle_T SFTM_CreateTimer(void)
{
  uint8_t newTimerNumber = 0;

  if (CurrentTimersNumber < MAX_TIMER_SLOTS)
  {
    newTimerNumber = CurrentTimersNumber++;
  }
  else
  {
    SFTM_ExecuteHardFault();
  }

  return (SFTM_TimerHandle_T)&TimersArray[newTimerNumber];
}

SFTM_TimerRet_T SFTM_StartTimer(SFTM_TimerHandle_T timerHandle, SFTM_TimerType_T timerType, SFTM_TimerCallback_T onExpire, void* pContext, SFTM_timeoutMS timeout)
{
  SFTM_TimerRet_T ret;

  if (timerHandle->ticks <= timerHandle->timeout)
  {
    ret = SFTM_TIMER_IN_USE;
  }
  else
  {
    timerHandle->timerType    = timerType;
    timerHandle->onExpire     = onExpire;
    timerHandle->pContext     = pContext;
    timerHandle->timeout      = timeout;
    timerHandle->ticks        = 0;
    timerHandle->expiredFlag  = false;

    ret = SFTM_TIMER_STARTED;
  }

  return ret;
}

void SFTM_StopTimer(SFTM_TimerHandle_T timerHandle)
{
  timerHandle->ticks        = TIMIER_IDLE_VALUE;
  timerHandle->timeout      = 0;
  timerHandle->expiredFlag  = false;
  timerHandle->onExpire     = NULL;
  timerHandle->pContext     = NULL;
}

void SFTM_RestartTimer(SFTM_TimerHandle_T timerHandle)
{
  timerHandle->ticks        = 0;
  timerHandle->expiredFlag  = false;
}

void SFTM_TimersEventsHandler(void)
{
  for (uint8_t timerCnt = 0; timerCnt < MAX_TIMER_SLOTS; timerCnt++)
  {
    if (true == TimersArray[timerCnt].expiredFlag)
    {
      /* Call timer event if is not NULL */
      if (TimersArray[timerCnt].onExpire != NULL)
      {
        TimersArray[timerCnt].onExpire(TimersArray[timerCnt].pContext);
      }
      else { /* Do nothing */ }

      if (SFTM_ONE_SHOT == TimersArray[timerCnt].timerType)
      {
        /* No more calls onExpire function */
        TimersArray[timerCnt].expiredFlag = false;
      }
      else // SFTM_AUTO_RELOAD
      {
        SFTM_RestartTimer(&TimersArray[timerCnt]);
      }
    }
    else
    {
      /* Do nothing */
    }
  }
}

SFTM_TimerStatus_T SFTM_GetTimerStatus(SFTM_TimerHandle_T timerHandle)
{
  if (timerHandle->ticks > timerHandle->timeout)
  {
    return SFTM_EXPIRED;
  }
  else
  {
    return SFTM_NOT_EXPIRED;
  }
}

uint32_t SFTM_GetTimerTick(SFTM_TimerHandle_T timerHandle)
{
  return timerHandle->ticks;
}

uint8_t SFTM_GetCurrentTimersNumberInSystem(void)
{
  return CurrentTimersNumber;
}

uint8_t SFTM_MaxTimersNumberInSystem(void)
{
  return MAX_TIMER_SLOTS;
}

/**
 * @}
 */
