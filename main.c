#include <stdint.h>

typedef uint32_t u32;

// define registers (base address + offset from the reference manual)
#define RCC_BASE    0x40023800UL
#define GPIOA_BASE  0x40020000UL
#define USART1_BASE 0x40011000UL

#define RCC_AHB1ENR (*(volatile u32 *)(RCC_BASE + 0x30))
#define GPIOA_MODER (*(volatile u32 *)(GPIOA_BASE + 0x00))
#define GPIOA_ODR   (*(volatile u32 *)(GPIOA_BASE + 0x14))

#define LED_PIN 5

int         main(void);
static void gpio_init(void);
static void delay(volatile u32 count);

int main(void) {
    gpio_init();

    while (1) {
        GPIOA_ODR ^= (1U << LED_PIN);
        delay(500000);
    }
}

static void gpio_init(void) {
    RCC_AHB1ENR |= (1U << 0); // enable to GPIOA's clock
    GPIOA_MODER &= ~(3U << (LED_PIN * 2));
    GPIOA_MODER |= (1U << (LED_PIN * 2));
}

static void delay(volatile u32 count) {
    while (count--)
        ;
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
