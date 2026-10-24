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

int main(void) {
    gpio_init();
    usart2_init();

    while (1) {
        usart2_write_str("hello\r\n");
        delay(500000);
    }
}

// note for GPIOA_MODER, pin n -> [2n+1 : 2n]
static void gpio_init(void) {
    RCC_AHB1ENR |= (1U << 0);                // enable to GPIOA's clock
    RCC_APB1ENR |= (1U << (USART2_REG * 1)); // enable USART2 clock
    GPIOA_MODER &= ~(3U << (PA2 * 2)); // clear bits (pin * field width) [5 : 4]
    GPIOA_MODER |= (2U << (PA2 * 2));  // alternative mode

    GPIOA_AFRL &= ~(15U << (PA2 * 4)); // clear AF7 bits. datasheet (AF7 -> PA2)
    GPIOA_AFRL |= (7U << (PA2 * 4));   // set AF7 bits
}

// baud = 16 MHz / (16 * USARTDIV) -> USARTDIV = 8.68
// BRR = mantissa 8, fraction 0.68 * 16 = 11 -> (8 << 4) | 11 = 139
static void usart2_init(void) {
    USART2_BRR = 139;                         // 115200 baud
    USART2_CR1 = USART_CR1_TE | USART_CR1_UE; // enable transmitter + USART
}

static void usart2_write_char(char c) {
    // wait until DR is free
    while (!(USART2_SR & USART_SR_TXE)) {
        USART2_DR = (u32)c;
    }
}

static void usart2_write_str(const char *s) {
    while (*s) {
        usart2_write_char(*s++);
    }
}

static void delay(volatile u32 count) {
    while (count--) {
    }
}

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
