#ifndef STM32_H
#define STM32_H
#include <stdint.h>

typedef uint32_t u32;

// define registers (base address + offset from the reference manual)
#define RCC_BASE    0x40023800UL
#define GPIOA_BASE  0x40020000UL
#define USART2_BASE 0x40004400UL

// base + offset
#define RCC_AHB1ENR (*(volatile u32 *)(RCC_BASE + 0x30))
#define GPIOA_MODER (*(volatile u32 *)(GPIOA_BASE + 0x00))
#define GPIOA_ODR   (*(volatile u32 *)(GPIOA_BASE + 0x14))
#define GPIOA_AFRL  (*(volatile u32 *)(GPIOA_BASE + 0x20))


// ------ usart ------
#define RCC_APB1ENR (*(volatile u32 *)(RCC_BASE + 0x40))
#define PA2         2
#define USART2_REG  17

#define USART2_SR  (*(volatile u32 *)(USART2_BASE + 0x00))
#define USART2_DR  (*(volatile u32 *)(USART2_BASE + 0x04))
#define USART2_BRR (*(volatile u32 *)(USART2_BASE + 0x08))
#define USART2_CR1 (*(volatile u32 *)(USART2_BASE + 0x0C))

#define USART_SR_TXE (1U << 7)  // transmit data register empty
#define USART_CR1_TE (1U << 3)  // transmitter enable
#define USART_CR1_UE (1U << 13) // USART enable
// ------ usart ------
#define LED_PIN 5

int         main(void);
static void gpio_init(void);
static void usart2_init(void);
static void usart2_write_char(char c);
static void usart2_write_str(const char *s);
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
#endif
