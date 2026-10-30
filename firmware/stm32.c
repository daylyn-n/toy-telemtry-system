#include "stm32.h"
#include "i2c.h"
#include "uart.h"

int         main(void);
static void delay(volatile u32 count);
static void usart2_write_hex(u8 value);

#define MPU6050_ADDR     0x68 // AD0 low
#define MPU6050_WHO_AM_I 0x75 // register; reads back 0x68

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
        u8 id = i2c_read_reg(MPU6050_ADDR, MPU6050_WHO_AM_I);

        usart2_write_str("WHO_AM_I = ");
        usart2_write_hex(id);
        usart2_write_str("\r\n");

        delay(500000);
    }
}

static void usart2_write_hex(u8 value) {
    const char *digits = "0123456789ABCDEF";

    usart2_write_str("0x");
    usart2_write_char(digits[value >> 4]);
    usart2_write_char(digits[value & 0x0F]);
}

static void delay(volatile u32 count) {
    while (count--) {
    }
}
