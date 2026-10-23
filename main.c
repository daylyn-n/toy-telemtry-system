#include <stdint.h>

typedef uint32_t u32;
// define registers (base address + offset from the refernce manual)

#define RCC_BASE   0x40023800UL
#define GPIOA_BASE 0x40020000UL


#define RCC_AHB1ENR (*(volatile u32 *)(RCC_BASW + 0x30))
#define GPIOA_MODER (*(volatile u32 *)(GPIOA_BASE + 0x00))
#define GPIOA_ODR   (*(volatile u32 *)(GPIOA_BASE + 0x14))


#define LED_PIN 5


static void gpio_init(void);
static void delay(volatile u32 count);
