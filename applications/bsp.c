/*****************************************************************************
 * BSP for EK-TM4C123GXL with QP/C framework
 *****************************************************************************/
#include "qpc.h" /* QP/C API */
#include "bsp.h"
#include "board.h"
#include "blinky.h" /* Blinky Application interface */
#include "led.h"
#include "lptimer.h"
#include "usart.h"
#include "cm_backtrace.h"

static bool sleep = false;

/* Assertion handler  ======================================================*/
Q_NORETURN Q_onAssert(char const* module, int_t id)
{
    /* TBD: Perform corrective actions and damage control
     * SPECIFIC to your particular system.
     */
    (void)module; /* unused parameter */
    (void)id;     /* unused parameter */

#ifndef NDEBUG  /* debug build? */
    while (1) { /* tie the CPU in this endless loop */
    }
#endif
    NVIC_SystemReset(); /* reset the CPU */
}
//............................................................................
void assert_failed(uint8_t* file, uint32_t line)
{
    printf("assert_failed at file: %s, line: %d\r\n", file, line);
#ifndef NDEBUG  /* debug build? */
    while (1) { /* tie the CPU in this endless loop */
    }
#endif
    NVIC_SystemReset(); /* reset the CPU */
}

/* ISRs  ===============================================*/
void SysTick_Handler(void)
{
    QTIMEEVT_TICK_X(0U, &l_SysTick_Handler); // time events at rate 0
    QV_ARM_ERRATUM_838869();
}

static void wakeup_handle(uint8_t bit)
{
    if (bit == 1) {
        sleep = false;
        printf("sleep\r\n");
    } else {
        sleep = true;
        printf("wakeup\r\n");
    }
}

/*..........................................................................*/
void QV_onIdle(void)
{
    if (sleep) {       
        HAL_SuspendTick();
        /* Enter STOP 2 mode */
        // HAL_PWREx_EnterSTOPMode(PWR_REGULATOR_VOLTAGE_SCALE0, PWR_STOPENTRY_WFI, PWR_D1_DOMAIN);
        /* Resume SysTick */
        HAL_ResumeTick();
        extern void SystemClock_Config(void);
        SystemClock_Config();
        SystemCoreClockUpdate();
    }else {
        HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
    }

    QF_INT_ENABLE(); /* just enable interrupts */
}

/* BSP functions ===========================================================*/
void BSP_init(void)
{
    /* NOTE: SystemInit() has been already called from the startup code
     *  but SystemCoreClock needs to be updated
     */
    cm_backtrace_init("build/stm32h7xx", "V1.0", "1.0.0");
    board_init();
    led_init();   /* initialize the LEDs */
    usart_init(); /* initialize the USART */
    printf("BSP_init: SystemCoreClock = %lu Hz\n", SystemCoreClock);
    // lptimer_init();
    // wakeup_init(wakeup_handle);
}

void BSP_start(void)
{
    // initialize event pools
    static QF_MPOOL_EL(QEvt) smlPoolSto[10];
    QF_poolInit(smlPoolSto, sizeof(smlPoolSto), sizeof(smlPoolSto[0]));

    // initialize publish-subscribe
    static QSubscrList subscrSto[MAX_PUB_SIG];
    QActive_psInit(subscrSto, Q_DIM(subscrSto));

    // instantiate and start AOs/threads...
    static QEvtPtr blinkyQueueSto[10];
    Blinky_ctor();
    QActive_start(AO_Blinky,
                  1U,                    // QP prio. of the AO
                  blinkyQueueSto,        // event queue storage
                  Q_DIM(blinkyQueueSto), // queue length [events]
                  (void*)0, 0U,          // no stack storage
                  (void*)0);             // no initialization param
}

/*..........................................................................*/
void QF_onStartup(void)
{
    // SysTick_Config(SystemCoreClock / BSP_TICKS_PER_SEC);
    // NVIC_SetPriority(LPTIM1_IRQn, 1);
    // NVIC_SetPriority(EXTI0_IRQn, 1);
    // NVIC_EnableIRQ(LPTIM1_IRQn);
    // NVIC_EnableIRQ(EXTI0_IRQn);
}
/*..........................................................................*/
void QF_onCleanup(void)
{
}

/*..........................................................................*/
void BSP_ledOn(void)
{
    led_on(LED_1);
}

/*..........................................................................*/
void BSP_ledOff(void)
{
    led_off(LED_1);
}
