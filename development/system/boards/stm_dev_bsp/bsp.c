#include "bsp.h"
#define max(a,b) (a)>(b)?a:b

static int SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef  ret = HAL_OK;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM       = 8;
  RCC_OscInitStruct.PLL.PLLN       = 200;
  RCC_OscInitStruct.PLL.PLLP       = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ       = 7;
  RCC_OscInitStruct.PLL.PLLR       = 2;

  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if (ret != HAL_OK) {
    return WST_ERR;
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3);
  if (ret != HAL_OK) {
    return WST_ERR;
  }

  return WST_OK;
}

void bsp_board_bringup(void)
{
  HAL_Init();

  WST_ERR_CHECK(SystemClock_Config());
}

/************************************************************
 *************************** UART ***************************
 ***********************************************************/
int bsp_uart_init(wst_uart_handle_t *handle)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  WST_UART_TX_GPIO_CLK_ENABLE();
  WST_UART_RX_GPIO_CLK_ENABLE();

  WST_UART_CLK_ENABLE();

  GPIO_InitStruct.Pin       = WST_UART_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = WST_UART_TX_AF;

  HAL_GPIO_Init(WST_UART_TX_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin       = WST_UART_RX_PIN;
  GPIO_InitStruct.Alternate = WST_UART_RX_AF;

  HAL_GPIO_Init(WST_UART_RX_GPIO_PORT, &GPIO_InitStruct);

  handle->Instance = WST_UART;

  handle->Init.BaudRate     = WST_UART_BAUDRATE;
  handle->Init.WordLength   = UART_WORDLENGTH_8B;
  handle->Init.StopBits     = UART_STOPBITS_1;
  handle->Init.Parity       = UART_PARITY_NONE;
  handle->Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  handle->Init.Mode         = UART_MODE_TX_RX;
  handle->Init.OverSampling = UART_OVERSAMPLING_16;

  if (HAL_UART_Init(handle) != HAL_OK) {
    return WST_ERR;
  }
  return WST_OK;
}

void bsp_uart_put_char(wst_uart_handle_t *handle, uint8_t *ch)
{
  HAL_UART_Transmit(handle, ch, 1, 0xFFFF);
}

void bsp_uart_write(wst_uart_handle_t *handle, uint8_t *data, uint16_t length)
{
  HAL_UART_Transmit(handle, data, length, 0xFFFF);
}

/************************************************************
 *************************** I2C ****************************
 ***********************************************************/
int bsp_i2c_init(wst_i2c_handle_t *handle)
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

  handle->Instance             = WST_I2C;
  handle->Init.ClockSpeed      = WST_I2C_SPEEDCLOCK;
  handle->Init.DutyCycle       = WST_I2C_DUTYCYCLE;
  handle->Init.OwnAddress1     = 0xFF;
  handle->Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
  handle->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  handle->Init.OwnAddress2     = 0xFF;
  handle->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  handle->Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;

  if(HAL_I2C_Init(handle) != HAL_OK) {
    return WST_ERR;
  }
  return WST_OK;
}

int bsp_i2c_write(wst_i2c_handle_t *handle,
             uint8_t slave_addr,
             uint8_t reg_addr,
             uint16_t length,
             uint8_t *data)
{
  while(HAL_I2C_Mem_Write(handle, ((uint16_t)slave_addr<<1), reg_addr, 1U, data, length, 25U) != HAL_OK) {
    if (HAL_I2C_GetError(handle) != HAL_I2C_ERROR_NONE) {
      return WST_ERR;
    }
  }
  return WST_OK;
}

int bsp_i2c_read(wst_i2c_handle_t *handle,
             uint8_t slave_addr,
             uint8_t reg_addr,
             uint16_t length,
             uint8_t *data)
{
  while(HAL_I2C_Mem_Read(handle, ((uint16_t)slave_addr<<1), reg_addr, 1U, data, length, 25U) != HAL_OK) {
    if (HAL_I2C_GetError(handle) != HAL_I2C_ERROR_NONE) {
      return WST_ERR;
    }
  }
  return WST_OK;
}

void error_handler(void)
{
  while (1);
}
