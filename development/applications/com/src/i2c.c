#include "i2c.h"
#include "bsp.h"

static wst_i2c_handle_t handle;

void i2c_init(void)
{
  WST_ERR_CHECK(bsp_i2c_init(&handle));
}

void i2c_write(uint8_t slave_addr,
               uint8_t reg_addr,
               uint16_t length,
               uint8_t *tx_buf)
{
  WST_ERR_CHECK(bsp_i2c_write(&handle, slave_addr, reg_addr, length, tx_buf));
}

void i2c_read(uint8_t slave_addr,
               uint8_t reg_addr,
               uint16_t length,
               uint8_t *rx_buf)
{
  WST_ERR_CHECK(bsp_i2c_read(&handle, slave_addr, reg_addr, length, rx_buf));
}