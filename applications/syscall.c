#include <main.h>
#include <stdio.h>

#ifdef __GNUC__
#ifdef TINY_STDIO
static int __fputc(char c, FILE* file);

static FILE __stdio_out = FDEV_SETUP_STREAM(__fputc, NULL, NULL, _FDEV_SETUP_WRITE);

#ifdef __strong_reference
#define STDIO_ALIAS(x) __strong_reference(stdout, x);
#else
#define STDIO_ALIAS(x) FILE* const x = &__stdio_out;
#endif

FILE* const stdout = &__stdio_out;
STDIO_ALIAS(stderr);

static int __fputc(char c, FILE* file)
{
    USART1->TDR = (uint8_t)c;
    while ((USART1->ISR & UART_FLAG_TC) == RESET);
    return c;
}

#endif
#endif