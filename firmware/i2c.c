#include "i2c.h"

void i2c_init(void) {
    RCC_APB1ENR |= (1U << I2C1_REG);

    RCC_AHB1ENR |= (1U << GPIOB_REG);
    GPIOB_MODER &= ~(3U << (PB8 * 2));
    GPIOB_MODER &= ~(3U << (PB9 * 2));
    GPIOB_MODER |= (2U << (PB8 * 2)); // alternate mode
    GPIOB_MODER |= (2U << (PB9 * 2)); // alternate mode
    GPIOB_AFRH |= (I2C1_AF << ((PB8 - 8) * 4));
    GPIOB_AFRH |= (I2C1_AF << ((PB9 - 8) * 4));

    // open-drain
    GPIOB_OTYPER &= ~(1U << PB8);
    GPIOB_OTYPER &= ~(1U << PB9);
    GPIOB_OTYPER |= (1U << PB8);
    GPIOB_OTYPER |= (1U << PB9);

    I2C1_CR1 &= ~I2C_CR1_PE; // CR2/CCR/TRISE must be set while disabled

    I2C1_CR2 &= ~I2C_CR2_FREQ_MASK;
    I2C1_CR2 |= 16U; // APB1 = 16 MHz

    // standard mode: T_high = T_low = CCR * T_pclk
    // 100 kHz -> 10 us period -> 5 us high -> CCR = 5000 ns / 62.5 ns = 80
    I2C1_CCR = 80; // FS = 0 (standard mode)

    // TRISE = (1000 ns max rise / 62.5 ns) + 1 = 17
    I2C1_TRISE = 17;

    I2C1_CR1 |= I2C_CR1_PE;
}

// read one byte from register `reg` on device `addr` (7-bit address)
uint8_t i2c_read_reg(u8 addr, u8 reg) {
    // wait until the bus is free
    while (I2C1_SR2 & I2C_SR2_BUSY)
        ;

    // ---- phase 1: tell the sensor which register we want ----
    I2C1_CR1 |= I2C_CR1_START;
    while (!(I2C1_SR1 & I2C_SR1_SB))
        ;

    I2C1_DR = (addr << 1) | 0; // 0 = write
    while (!(I2C1_SR1 & I2C_SR1_ADDR))
        ;
    (void)I2C1_SR1; // clear ADDR: read SR1 then SR2
    (void)I2C1_SR2;

    while (!(I2C1_SR1 & I2C_SR1_TXE))
        ;
    I2C1_DR = reg;
    while (!(I2C1_SR1 & I2C_SR1_BTF)) // register byte fully sent
        ;

    // ---- phase 2: read the register's value back ----
    I2C1_CR1 |= I2C_CR1_START; // repeated start
    while (!(I2C1_SR1 & I2C_SR1_SB))
        ;

    I2C1_DR = (addr << 1) | 1; // 1 = read
    while (!(I2C1_SR1 & I2C_SR1_ADDR))
        ;

    // single byte: NACK it so the sensor stops sending.
    // ACK must be cleared *before* ADDR is cleared.
    I2C1_CR1 &= ~I2C_CR1_ACK;
    (void)I2C1_SR1; // clear ADDR: read SR1 then SR2
    (void)I2C1_SR2;
    I2C1_CR1 |= I2C_CR1_STOP;

    while (!(I2C1_SR1 & I2C_SR1_RXNE))
        ;
    return (u8)I2C1_DR;
}
