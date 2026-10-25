#include "stm32.h"

int main(void) {
    gpio_init();
    usart2_init();
    usart1_init();

    while (1) {
        usart2_write_str("hello\r\n");
        usart1_write_str("hello esp32\r\n");
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

    // PA9 -> USART1 TX. pins 8-15 live in AFRH, so field = (pin - 8)
    RCC_APB2ENR |= (1U << USART1_REG); // enable USART1 clock
    GPIOA_MODER &= ~(3U << (PA9 * 2));
    GPIOA_MODER |= (2U << (PA9 * 2)); // alternate mode
    GPIOA_AFRH &= ~(15U << ((PA9 - 8) * 4));
    GPIOA_AFRH |= (7U << ((PA9 - 8) * 4)); // AF7 -> USART1
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
    }
    USART2_DR = (u32)c;
}

static void usart2_write_str(const char *s) {
    while (*s) {
        usart2_write_char(*s++);
    }
}

// APB2 also runs at 16 MHz out of reset, so same BRR as USART2
static void usart1_init(void) {
    USART1_BRR = 139;                         // 115200 baud
    USART1_CR1 = USART_CR1_TE | USART_CR1_UE; // enable transmitter + USART
}

static void usart1_write_char(char c) {
    while (!(USART1_SR & USART_SR_TXE)) {
    }
    USART1_DR = (u32)c;
}

static void usart1_write_str(const char *s) {
    while (*s) {
        usart1_write_char(*s++);
    }
}

static void delay(volatile u32 count) {
    while (count--) {
    }
}
