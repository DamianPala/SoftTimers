/*=======================================================================================*
 * @file    TC_SoftTimers.c
 * @author  Damian Pala
 * @date    29-01-2017
 * @brief   This file contains unit tests for Soft Timers module.
 *======================================================================================*/

/**
 * @addtogroup Soft Timers Unit Tests Description
 * @{
 * @brief Module for... .
 */

/*======================================================================================*/
/*                       ####### PREPROCESSOR DIRECTIVES #######                        */
/*======================================================================================*/
/*---------------------- INCLUDE DIRECTIVES FOR STANDARD HEADERS -----------------------*/

/*----------------------- INCLUDE DIRECTIVES FOR OTHER HEADERS -------------------------*/
#include "unity.h"
#include "unity_fixture.h"

#include "SoftTimers.c"

/*----------------------------- LOCAL OBJECT-LIKE MACROS -------------------------------*/

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
/*--------------------------------- EXPORTED OBJECTS -----------------------------------*/

/*---------------------------------- LOCAL OBJECTS -------------------------------------*/
TEST_GROUP(SoftTimers);
static uint32_t OnExpireCallsNumber = 0;

/*======================================================================================*/
/*                    ####### LOCAL FUNCTIONS PROTOTYPES #######                        */
/*======================================================================================*/
static void TimerOnExpireFunction(void);

/*======================================================================================*/
/*                   ####### LOCAL FUNCTIONS DEFINITIONS #######                        */
/*======================================================================================*/
static void TimerOnExpireFunction(void)
{
  OnExpireCallsNumber++;
}

/*======================================================================================*/
/*                        ####### TESTS DEFINITIONS #######                             */
/*======================================================================================*/
TEST_SETUP(SoftTimers)
{
  SFTM_Init();
  CurrentTimersNumber = 0;
  OnExpireCallsNumber = 0;
}

TEST_TEAR_DOWN(SoftTimers)
{

}

TEST(SoftTimers, SFTM_Init_should_InitializeTimersSlotsProperly)
{
  for (SFTM_TimerHandle_T timerHandle = FIRST_TIMER_HANDLE; timerHandle < MAX_TIMER_SLOTS + FIRST_TIMER_HANDLE; timerHandle++)
  {
    TEST_ASSERT_EQUAL_UINT32(TIMIER_IDLE_VALUE, TimersArray[GET_TIMER_SLOT(timerHandle)].ticks);
    TEST_ASSERT_EQUAL_UINT32(0, TimersArray[GET_TIMER_SLOT(timerHandle)].timeout);
    TEST_ASSERT_FALSE(TimersArray[GET_TIMER_SLOT(timerHandle)].expiredFlag);
    TEST_ASSERT_NULL(TimersArray[GET_TIMER_SLOT(timerHandle)].onExpire);
  }
}

TEST(SoftTimers, Timer_should_CallOnExpireOnlyOneTimeWhenItIsSingleShotType)
{
  const uint32_t timeout = 7;
  const uint32_t periodNumber = 2000;
  uint32_t timersHandlerTicks = TICK_CMP * timeout * periodNumber;
  SFTM_TimerHandle_T testedTimer;

  testedTimer = SFTM_CreateTimer();
  SFTM_StartTimer(testedTimer, SFTM_ONE_SHOT, TimerOnExpireFunction, timeout);
  for (uint32_t cnt = 0; cnt < timersHandlerTicks; cnt++)
  {
    SFTM_TimersHandler();
    SFTM_TimersEventsHandler();
  }
  TEST_ASSERT_EQUAL_UINT32(1, OnExpireCallsNumber);
}

TEST(SoftTimers, Timer_should_CallOnExpireWhenTimerReachesTimeoutOnFirstTimerSlot)
{
  const uint32_t timeout = 10;
  uint32_t timersHandlerTicks = TICK_CMP * timeout;
  SFTM_TimerHandle_T testedTimer;

  testedTimer = SFTM_CreateTimer();
  SFTM_StartTimer(testedTimer, SFTM_ONE_SHOT, TimerOnExpireFunction, timeout);
  for (uint32_t cnt = 0; cnt < timersHandlerTicks; cnt++)
  {
    SFTM_TimersHandler();
    SFTM_TimersEventsHandler();
  }
  TEST_ASSERT_EQUAL_UINT32(1, OnExpireCallsNumber);
}

TEST(SoftTimers, Timer_should_CallOnExpireWhenTimerReachesTimeoutOnLastTimerSlot)
{
  const uint32_t timeout = 10;
  uint32_t timersHandlerTicks = TICK_CMP * timeout;
  SFTM_TimerHandle_T testedTimersArray[MAX_TIMER_SLOTS];
  SFTM_TimerHandle_T testedTimer;

  for (uint8_t cnt = 0; cnt < MAX_TIMER_SLOTS; cnt++)
  {
    testedTimersArray[cnt] = SFTM_CreateTimer();
  }

  testedTimer = testedTimersArray[MAX_TIMER_SLOTS - 1];
  SFTM_StartTimer(testedTimer, SFTM_ONE_SHOT, TimerOnExpireFunction, timeout);
  for (uint32_t cnt = 0; cnt < timersHandlerTicks; cnt++)
  {
    SFTM_TimersHandler();
    SFTM_TimersEventsHandler();
  }
  TEST_ASSERT_EQUAL_UINT32(1, OnExpireCallsNumber);
}

TEST(SoftTimers, Timer_should_CallOnExpireEveryExpirationWhenItIsAutoreloaded)
{
  const uint32_t timeout = 8;
  const uint32_t periodNumber = 2000;
  uint32_t timersHandlerTicks = TICK_CMP * timeout * periodNumber;
  SFTM_TimerHandle_T testedTimer;

  testedTimer = SFTM_CreateTimer();
  SFTM_StartTimer(testedTimer, SFTM_AUTO_RELOAD, TimerOnExpireFunction, timeout);
  for (uint32_t cnt = 0; cnt < timersHandlerTicks; cnt++)
  {
    SFTM_TimersHandler();
    SFTM_TimersEventsHandler();
  }
  TEST_ASSERT_EQUAL_UINT32(periodNumber, OnExpireCallsNumber);
}

// TODO:
TEST(SoftTimers, Timer_should_OperateIndependently)
{
//  const uint32_t timeout = 8;
//  const uint32_t periodNumber = 2000;
//  uint32_t timersHandlerTicks = TICK_CMP * timeout * periodNumber;
//  SFTM_TimerHandle_T testedTimer;
//
//  testedTimer = SFTM_CreateTimer();
//  SFTM_StartTimer(testedTimer, SFTM_AUTO_RELOAD, TimerOnExpireFunction, timeout);
//  for (uint32_t cnt = 0; cnt < timersHandlerTicks; cnt++)
//  {
//    SFTM_TimersHandler();
//    SFTM_TimersEventsHandler();
//  }
//  TEST_ASSERT_EQUAL_UINT32(periodNumber, OnExpireCallsNumber);
}

/**
 * @}
 */
