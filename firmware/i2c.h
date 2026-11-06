#ifndef I2C_H
#define I2C_H
#include "stm32.h"
#include <stdint.h>

void i2c_init(void);
// returns 0 on success (byte stored in *out), -1 on timeout / no ACK
int  i2c_read_reg(u8 addr, u8 reg, u8 *out);

#endif
