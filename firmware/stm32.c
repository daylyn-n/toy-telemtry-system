#include "stm32.h"
#include "i2c.h"
#include "uart.h"

int         main(void);
static void delay(volatile u32 count);
static void send_line(const char *s);
static void to_hex(u8 value, char *out);

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
        u8 id;
        if (i2c_read_reg(MPU6050_ADDR, MPU6050_WHO_AM_I, &id) == 0) {
            char line[] = "WHO_AM_I = 0x??\r\n";
            to_hex(id, &line[13]);
            send_line(line);
        } else {
            send_line("IMU ERROR: no response\r\n");
        }

        delay(500000);
    }
}

// same line to the PC monitor (USART2) and the ESP32 (USART1)
static void send_line(const char *s) {
    usart2_write_str(s);
    usart1_write_str(s);
}

// write a byte as two hex characters into out[0], out[1]
static void to_hex(u8 value, char *out) {
    const char *digits = "0123456789ABCDEF";

    out[0] = digits[value >> 4];
    out[1] = digits[value & 0x0F];
}

static void delay(volatile u32 count) {
    while (count--) {
    }
}
