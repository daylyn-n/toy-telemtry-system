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

// one I2C byte at 100 kHz takes ~90 us; this gives each wait a few ms
#define I2C_TIMEOUT 50000U

// spin until (*reg & mask) == want, but give up after I2C_TIMEOUT tries.
// also bails early if the sensor didn't ACK (AF flag).
static int i2c_wait(volatile u32 *reg, u32 mask, u32 want) {
    for (u32 i = 0; i < I2C_TIMEOUT; i++) {
        if ((*reg & mask) == want) {
            return 0;
        }
        if (I2C1_SR1 & I2C_SR1_AF) {
            return -1;
        }
    }
    return -1;
}

// something went wrong: release the bus and reset the peripheral so the
// next call starts clean instead of seeing BUSY forever
static void i2c_recover(void) {
    I2C1_CR1 |= I2C_CR1_STOP;
    I2C1_SR1 &= ~I2C_SR1_AF;
    I2C1_CR1 |= I2C_CR1_SWRST;
    I2C1_CR1 &= ~I2C_CR1_SWRST;
    i2c_init();
}

// read one byte from register `reg` on device `addr` (7-bit address)
int i2c_read_reg(u8 addr, u8 reg, u8 *out) {
    // wait until the bus is free
    if (i2c_wait(&I2C1_SR2, I2C_SR2_BUSY, 0))
        goto fail;

    // ---- phase 1: tell the sensor which register we want ----
    I2C1_CR1 |= I2C_CR1_START;
    if (i2c_wait(&I2C1_SR1, I2C_SR1_SB, I2C_SR1_SB))
        goto fail;

    I2C1_DR = (u32)(addr << 1) | 0U; // 0 = write
    if (i2c_wait(&I2C1_SR1, I2C_SR1_ADDR, I2C_SR1_ADDR))
        goto fail;
    (void)I2C1_SR1; // clear ADDR: read SR1 then SR2
    (void)I2C1_SR2;

    if (i2c_wait(&I2C1_SR1, I2C_SR1_TXE, I2C_SR1_TXE))
        goto fail;
    I2C1_DR = reg;
    if (i2c_wait(&I2C1_SR1, I2C_SR1_BTF, I2C_SR1_BTF)) // byte fully sent
        goto fail;

    // ---- phase 2: read the register's value back ----
    I2C1_CR1 |= I2C_CR1_START; // repeated start
    if (i2c_wait(&I2C1_SR1, I2C_SR1_SB, I2C_SR1_SB))
        goto fail;

    I2C1_DR = (u32)(addr << 1) | 1U; // 1 = read
    if (i2c_wait(&I2C1_SR1, I2C_SR1_ADDR, I2C_SR1_ADDR))
        goto fail;

    // single byte: NACK it so the sensor stops sending.
    // ACK must be cleared *before* ADDR is cleared.
    I2C1_CR1 &= ~I2C_CR1_ACK;
    (void)I2C1_SR1; // clear ADDR: read SR1 then SR2
    (void)I2C1_SR2;
    I2C1_CR1 |= I2C_CR1_STOP;

    if (i2c_wait(&I2C1_SR1, I2C_SR1_RXNE, I2C_SR1_RXNE))
        goto fail;
    *out = (u8)I2C1_DR;
    return 0;

fail:
    i2c_recover();
    return -1;
}
