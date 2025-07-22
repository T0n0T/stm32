/*****************************************************************************
 * BSP for EK-TM4C123GXL with QP/C framework
 *****************************************************************************/
#include "qpc.h" /* QP/C API */
#include "bsp.h"
#include "board.h"
#include "blinky.h" /* Blinky Application interface */
#include "led.h"
#include "button.h"
#include "usart.h"
#include "cm_backtrace.h"

#define BTN_SW1 (1U << 4)
#define BTN_SW2 (1U << 0)

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
/* assert-handling function called by exception handlers in the startup code */
void assert_failed(char const* const module, int_t const id); // prototype
void assert_failed(char const* const module, int_t const id)
{
    Q_onAssert(module, id);
}

/* ISRs  ===============================================*/
void SysTick_Handler(void)
{
    QTIMEEVT_TICK_X(0U, &l_SysTick_Handler); // time events at rate 0

#ifdef Q_SPY
    uint32_t volatile tmp = SysTick->CTRL; // clear CTRL_COUNTFLAG
    QS_tickTime_ += QS_tickPeriod_;        // account for the clock rollover
    Q_UNUSED_PAR(tmp);
#endif

    QV_ARM_ERRATUM_838869();
}

/*..........................................................................*/
void QV_onIdle(void)
{
#ifdef NDEBUG
    /* Put the CPU and peripherals to the low-power mode.
     * you might need to customize the clock management for your application,
     * see the datasheet for your particular Cortex-M MCU.
     */
    QV_CPU_SLEEP(); /* atomically go to sleep and enable interrupts */
#else
    QF_INT_ENABLE(); /* just enable interrupts */
#endif
}

/* BSP functions ===========================================================*/
void BSP_init(void)
{
    /* NOTE: SystemInit() has been already called from the startup code
     *  but SystemCoreClock needs to be updated
     */
    cm_backtrace_init("STM32L4", "V1.0", "1.0.0");
    board_init();
    led_init();   /* initialize the LEDs */
    usart_init(); /* initialize the USART */
    printf("BSP_init: SystemCoreClock = %lu Hz\n", SystemCoreClock);
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
    /* set up the SysTick timer to fire at BSP_TICKS_PER_SEC rate
     * NOTE: do NOT call OS_CPU_SysTickInit() from uC/OS-II
     */
    SysTick_Config(SystemCoreClock / BSP_TICKS_PER_SEC);

    /* set priorities of ALL ISRs used in the system, see NOTE1 */
    NVIC_SetPriority(SysTick_IRQn, QF_AWARE_ISR_CMSIS_PRI + 1U);
    /* ... */

    /* enable IRQs in the NVIC... */
    /* ... */
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
/*..........................................................................*/
void BSP_ledBlueOn(void)
{
    led_on(LED_2);
}

/*..........................................................................*/
void BSP_ledBlueOff(void)
{
    led_off(LED_2);
}

/*..........................................................................*/
void BSP_ledGreenOn(void)
{
    led_on(LED_3);
}

/*..........................................................................*/
void BSP_ledGreenOff(void)
{
    led_off(LED_3);
}

#define TRACE_CHANNEL 1
#define DELAY_TIME    40

__attribute__((no_instrument_function)) void __cyg_profile_func_enter(void* this_fn, void* call_site)
{
    if (!(ITM->TER & (1 << TRACE_CHANNEL))) return;
    uint32_t oldIntStat = __get_PRIMASK();

    // This is not atomic, but by using the stack for
    // storing oldIntStat it doesn't matter
    __disable_irq();
    while (ITM->PORT[TRACE_CHANNEL].u32 == 0);

    // This is CYCCNT - number of cycles of the CPU clock
    ITM->PORT[TRACE_CHANNEL].u32 = ((*((uint32_t*)0xE0001004)) & 0x03FFFFFF) | 0x40000000;
    while (ITM->PORT[TRACE_CHANNEL].u32 == 0);
    ITM->PORT[TRACE_CHANNEL].u32 = (uint32_t)(call_site) & 0xFFFFFFFE;
    while (ITM->PORT[TRACE_CHANNEL].u32 == 0);

    ITM->PORT[TRACE_CHANNEL].u32 = (uint32_t)this_fn & 0xFFFFFFFE;
    for (uint32_t d = 0; d < DELAY_TIME; d++) asm volatile("NOP");

    __set_PRIMASK(oldIntStat);
}

__attribute__((no_instrument_function)) void __cyg_profile_func_exit(void* this_fn, void* call_site)
{
    if (!(ITM->TER & (1 << TRACE_CHANNEL))) return;
    uint32_t oldIntStat = __get_PRIMASK();
    __disable_irq();
    while (ITM->PORT[TRACE_CHANNEL].u32 == 0);
    ITM->PORT[TRACE_CHANNEL].u32 = ((*((uint32_t*)0xE0001004)) & 0x03FFFFFF) | 0x50000000;
    while (ITM->PORT[TRACE_CHANNEL].u32 == 0);
    ITM->PORT[TRACE_CHANNEL].u32 = (uint32_t)(call_site) & 0xFFFFFFFE;
    while (ITM->PORT[TRACE_CHANNEL].u32 == 0);
    ITM->PORT[TRACE_CHANNEL].u32 = (uint32_t)this_fn & 0xFFFFFFFE;
    for (uint32_t d = 0; d < DELAY_TIME; d++) asm volatile("NOP");
    __set_PRIMASK(oldIntStat);
}