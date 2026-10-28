#ifndef I2C_H
#define I2C_H
#include "stm32.h"
#include <stdint.h>

void    i2c_init(void);
uint8_t i2c_read_reg(u8 addr, u8 reg);

#endif
