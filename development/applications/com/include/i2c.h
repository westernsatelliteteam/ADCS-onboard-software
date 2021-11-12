#ifndef I2C_H
#define I2C_H

#include "com.h"
#include "bsp.h"

void i2c_init(void);
void i2c_write(uint8_t slave_addr,
               uint8_t reg_addr,
               uint16_t length,
               uint8_t *tx_buf);
void i2c_read(uint8_t slave_addr,
               uint8_t reg_addr,
               uint16_t length,
               uint8_t *rx_buf);

#endif // I2C_H
