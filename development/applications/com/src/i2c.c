#include "i2c.h"

#include "board.h"
#include "wst_common.h"

#define TIMEOUT 25

static I2C_HandleTypeDef sensors_i2c_handle;

static int write(I2C_HandleTypeDef *handle, uint8_t slave_addr,
          uint8_t reg_addr, const uint8_t *data, uint16_t length)
{
  while (HAL_I2C_Mem_Write(handle, ((uint16_t)slave_addr<<1), reg_addr, 1U, data, length, TIMEOUT)) {
    if (HAL_I2C_GetError(handle) != HAL_I2C_ERROR_AF) {
      return WST_ERR;
    }
  }
  return WST_OK;
}

static int read(I2C_HandleTypeDef *handle, uint8_t slave_addr,
         uint8_t reg_addr, uint8_t *data, uint16_t length)
{
  while (HAL_I2C_Mem_Read(handle, ((uint16_t)slave_addr<<1), reg_addr, 1U, data, length, TIMEOUT)) {
    if (HAL_I2C_GetError(handle) != HAL_I2C_ERROR_AF) {
      return WST_ERR;
    }
  }
  return WST_OK;
}

int i2c_imu_write(uint8_t slave_addr, uint8_t reg_addr,
                  const uint8_t *data, uint16_t length)
{
  return write(&sensors_i2c_handle, slave_addr, reg_addr, data, length);
}

int i2c_imu_read(uint8_t slave_addr, uint8_t reg_addr,
                 uint8_t *data, uint16_t length)
{
  return read(&sensors_i2c_handle, slave_addr, reg_addr, data, length);
}

int i2c_init(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  WST_I2C_SCL_GPIO_CLK_ENABLE();
  WST_I2C_SDA_GPIO_CLK_ENABLE();
  WST_I2C_CLK_ENABLE();

  WST_I2C_FORCE_RESET();
  WST_I2C_RELEASE_RESET();

  GPIO_InitStruct.Pin       = WST_I2C_SCL_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = WST_I2C_SCL_SDA_AF;
  HAL_GPIO_Init(WST_I2C_SCL_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin       = WST_I2C_SDA_PIN;
  GPIO_InitStruct.Alternate = WST_I2C_SCL_SDA_AF;
  HAL_GPIO_Init(WST_I2C_SDA_GPIO_PORT, &GPIO_InitStruct);

  sensors_i2c_handle.Instance             = WST_I2C;

  sensors_i2c_handle.Init.Timing = 0x00101A26;
  sensors_i2c_handle.Init.OwnAddress1 = 0;
  sensors_i2c_handle.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  sensors_i2c_handle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  sensors_i2c_handle.Init.OwnAddress2 = 0;
  sensors_i2c_handle.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  sensors_i2c_handle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  sensors_i2c_handle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;

  if(HAL_I2C_Init(&sensors_i2c_handle) != HAL_OK) {
    return WST_ERR;
  }
  return WST_OK;
}
