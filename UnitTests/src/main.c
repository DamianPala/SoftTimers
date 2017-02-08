/*=======================================================================================*
 * @file    main.c
 * @author  Damian Pala
 * @date    29-01-2017
 * @brief   This file contains test runner procedures for unit tests.
 *======================================================================================*/

/*======================================================================================*/
/*                       ####### PREPROCESSOR DIRECTIVES #######                        */
/*======================================================================================*/
/*---------------------- INCLUDE DIRECTIVES FOR STANDARD HEADERS -----------------------*/

/*----------------------- INCLUDE DIRECTIVES FOR OTHER HEADERS -------------------------*/
#include "unity_fixture.h"

/*======================================================================================*/
/*                           ####### TESTS GROUPS #######                               */
/*======================================================================================*/
TEST_GROUP_RUNNER(SoftTimers)
{
  RUN_TEST_CASE(SoftTimers, SFTM_Init_should_InitializeTimersSlotsProperly);
  RUN_TEST_CASE(SoftTimers, Timer_should_CallOnExpireOnlyOneTimeWhenItIsSingleShotType);
  RUN_TEST_CASE(SoftTimers, Timer_should_CallOnExpireWhenTimerReachesTimeoutOnFirstTimerSlot);
  RUN_TEST_CASE(SoftTimers, Timer_should_CallOnExpireWhenTimerReachesTimeoutOnLastTimerSlot);
  RUN_TEST_CASE(SoftTimers, Timer_should_CallOnExpireEveryExpirationWhenItIsAutoreloaded);
}

/*======================================================================================*/
/*                    ####### LOCAL FUNCTIONS PROTOTYPES #######                        */
/*======================================================================================*/
static void RunAllTests(void);

/*======================================================================================*/
/*                   ####### LOCAL FUNCTIONS DEFINITIONS #######                        */
/*======================================================================================*/
static void RunAllTests(void)
{
  RUN_TEST_GROUP(SoftTimers);
}

/*======================================================================================*/
/*                 ####### EXPORTED FUNCTIONS DEFINITIONS #######                       */
/*======================================================================================*/
int main(int argc, const char * argv[])
{
  return UnityMain(argc, argv, RunAllTests);
}
