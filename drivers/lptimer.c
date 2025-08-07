#include "lptimer.h"

LPTIM_HandleTypeDef LptimHandle;
RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct;
RCC_OscInitTypeDef RCC_OscInitStruct;

void lptimer_init(void)
{   
    /* Enable LSE clock */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.LSEState       = RCC_LSE_ON;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        assert_param(0);
    }

    /*  Initialize LPTIM peripheral ################################## */
    /*
     *  Instance        = LPTIM1.
     *  Clock Source    = APB or LowPowerOSCillator
     *  Counter source  = Internal event.
     *  Clock prescaler = 1 (No division).
     *  Counter Trigger = Trigger1: PB.06
     *  Active Edge     = Rising edge.
     */

    LptimHandle.Instance = LPTIM1;

    LptimHandle.Init.Clock.Source       = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
    LptimHandle.Init.Clock.Prescaler    = LPTIM_PRESCALER_DIV1;
    LptimHandle.Init.Trigger.Source     = LPTIM_TRIGSOURCE_SOFTWARE;
    LptimHandle.Init.Trigger.ActiveEdge = LPTIM_ACTIVEEDGE_RISING;
    LptimHandle.Init.CounterSource      = LPTIM_COUNTERSOURCE_INTERNAL;
    LptimHandle.Init.Input1Source       = LPTIM_INPUT1SOURCE_GPIO;
    LptimHandle.Init.Input2Source       = LPTIM_INPUT2SOURCE_GPIO;
    
    /* Initialize LPTIM peripheral according to the passed parameters */
    if (HAL_LPTIM_Init(&LptimHandle) != HAL_OK) {
        assert_param(0);
    }

    /*  Start the Timeout function in interrupt mode ################# */
    /*
     *  Period = 65535
     *  Pulse  = 32767
     *  According to this configuration (LPTIMER clocked by LSE & compare = 32767,
     *  the Timeout period = (compare + 1)/LSE_Frequency = 1s
     */
    if (HAL_LPTIM_TimeOut_Start_IT(&LptimHandle, 65535, 32768 - 1) != HAL_OK) {
        assert_param(0);
    }
}
