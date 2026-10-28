#ifndef STM32_H
#define STM32_H
#include <stdint.h>

typedef uint32_t u32;
typedef uint8_t  u8;

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

// ------ usart1 (to esp32) ------
#define USART1_BASE 0x40011000UL
#define RCC_APB2ENR (*(volatile u32 *)(RCC_BASE + 0x44))
#define GPIOA_AFRH  (*(volatile u32 *)(GPIOA_BASE + 0x24))
#define PA9         9
#define USART1_REG  4

#define USART1_SR  (*(volatile u32 *)(USART1_BASE + 0x00))
#define USART1_DR  (*(volatile u32 *)(USART1_BASE + 0x04))
#define USART1_BRR (*(volatile u32 *)(USART1_BASE + 0x08))
#define USART1_CR1 (*(volatile u32 *)(USART1_BASE + 0x0C))
// ------ usart1 (to esp32) ------

#define USART_SR_TXE (1U << 7)  // transmit data register empty
#define USART_CR1_TE (1U << 3)  // transmitter enable
#define USART_CR1_UE (1U << 13) // USART enable
// ------ usart ------

// ------ I2C to MPU 6050 -----
#define GPIOB_BASE 0x40020400UL
#define I2C1_BASE  0x40005400UL
#define GPIOB_REG  1  // AHB1ENR bit
#define I2C1_REG   21 // APB1ENR bit
#define PB8        8  // SCL
#define PB9        9  // SDA
#define I2C1_AF    4  // AF4 -> I2C1 on PB8/PB9

#define GPIOB_MODER (*(volatile u32 *)(GPIOB_BASE + 0x00))
#define GPIOB_OTYPER                                                           \
    (*(volatile u32 *)(GPIOB_BASE + 0x04)) // I2C pins must be open-drain
#define GPIOB_AFRH (*(volatile u32 *)(GPIOB_BASE + 0x24))

#define I2C1_CR1   (*(volatile u32 *)(I2C1_BASE + 0x00))
#define I2C1_CR2   (*(volatile u32 *)(I2C1_BASE + 0x04))
#define I2C1_DR    (*(volatile u32 *)(I2C1_BASE + 0x10))
#define I2C1_SR1   (*(volatile u32 *)(I2C1_BASE + 0x14))
#define I2C1_SR2   (*(volatile u32 *)(I2C1_BASE + 0x18))
#define I2C1_CCR   (*(volatile u32 *)(I2C1_BASE + 0x1C))
#define I2C1_TRISE (*(volatile u32 *)(I2C1_BASE + 0x20))

#define I2C_CR1_PE    (1U << 0)  // peripheral enable
#define I2C_CR1_START (1U << 8)  // generate start condition
#define I2C_CR1_STOP  (1U << 9)  // generate stop condition
#define I2C_CR1_ACK   (1U << 10) // ACK received bytes
#define I2C_CR1_SWRST (1U << 15) // software reset

#define I2C_CR2_FREQ_MASK 0x3FU // [5:0] APB1 clock in MHz

#define I2C_SR1_SB   (1U << 0)  // start bit sent
#define I2C_SR1_ADDR (1U << 1)  // address sent + ACKed
#define I2C_SR1_BTF  (1U << 2)  // byte transfer finished
#define I2C_SR1_RXNE (1U << 6)  // receive data register not empty
#define I2C_SR1_TXE  (1U << 7)  // transmit data register empty
#define I2C_SR1_AF   (1U << 10) // acknowledge failure (no ACK)

#define I2C_SR2_BUSY (1U << 1) // bus busy

#define I2C_CCR_FS                                                             \
    (1U << 15) // 0 = standard mode (100 kHz), 1 = fast mode (400 kHz)

// MPU-6050 (register map doc)
#define MPU_ADDR       0x68 // 7-bit address, AD0 low
#define MPU_PWR_MGMT_1 0x6B // write 0x00 to wake from sleep
#define MPU_WHO_AM_I   0x75 // should read back 0x68


// ------ I2C to MPU 6050 -----
#define LED_PIN 5

#endif
