#include "stm32.h"
#include "i2c.h"
#include "uart.h"

int         main(void);
static void delay(volatile u32 count);

// startup: runs on reset, before main
extern void _estack(void); // defined in f446re.ld

__attribute__((noreturn)) void _reset(void) {
    main();
    for (;;)
        (void)0;
}

/* 16 core entries + 91 STM32F446 interrupts */
__attribute__((section(".vectors"))) void (*const tab[16 + 91])(void) = {
    _estack, _reset};

int main(void) {
    usart2_init();
    usart1_init();
    i2c_init();

    while (1) {
        usart2_write_str("hello\r\n");
        usart1_write_str("hello esp32\r\n");
        delay(500000);
    }
}

static void delay(volatile u32 count) {
    while (count--) {
    }
}
