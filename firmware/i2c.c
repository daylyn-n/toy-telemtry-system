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
    // TODO: wait until the bus is free (SR2 BUSY = 0)

    // ---- phase 1: tell the sensor which register we want ----
    // TODO: set CR1 START, wait for SR1 SB
    // TODO: write (addr << 1) | 0 to DR  (0 = write)
    // TODO: wait for SR1 ADDR, then clear it by reading SR1 then SR2
    // TODO: wait for SR1 TXE, write `reg` to DR
    // TODO: wait for SR1 BTF (register byte fully sent)

    // ---- phase 2: read the register's value back ----
    // TODO: set CR1 START again (repeated start), wait for SR1 SB
    // TODO: write (addr << 1) | 1 to DR  (1 = read)
    // TODO: wait for SR1 ADDR
    // TODO: clear CR1 ACK *before* clearing ADDR -- for a single byte,
    //       the STM32 must NACK it so the sensor stops sending
    // TODO: clear ADDR (read SR1 then SR2)
    // TODO: set CR1 STOP
    // TODO: wait for SR1 RXNE, read the byte from DR and return it

    return 0;
}
