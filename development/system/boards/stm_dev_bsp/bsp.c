#include "bsp.h"
#define max(a,b) (a)>(b)?a:b

static int SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_9;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 71;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV6;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    // Error_Handler();
    while(1);
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV4;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    // Error_Handler();
    while(1);
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_LPUART1|RCC_PERIPHCLK_USB;
  PeriphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_PCLK1;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLLSAI1;
  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_MSI;
  PeriphClkInit.PLLSAI1.PLLSAI1M = 5;
  PeriphClkInit.PLLSAI1.PLLSAI1N = 20;
  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_48M2CLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    // Error_Handler();
    while(1);
  }
  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    // Error_Handler();
    while(1);
  }
  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();

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

  GPIO_InitStruct.Pin       = WST_UART_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = WST_UART_TX_AF;

  HAL_GPIO_Init(WST_UART_TX_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin       = WST_UART_RX_PIN;
  GPIO_InitStruct.Alternate = WST_UART_RX_AF;

  HAL_GPIO_Init(WST_UART_RX_GPIO_PORT, &GPIO_InitStruct);

  WST_UART_CLK_ENABLE();

  WST_UART_TX_GPIO_CLK_ENABLE();
  WST_UART_RX_GPIO_CLK_ENABLE();

  WST_UART_VDD_ENABLE();

  handle->Instance = WST_UART;

  handle->Init.BaudRate     = WST_UART_BAUDRATE;
  handle->Init.WordLength   = UART_WORDLENGTH_8B;
  handle->Init.StopBits     = UART_STOPBITS_1;
  handle->Init.Parity       = UART_PARITY_NONE;
  handle->Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  handle->Init.Mode         = UART_MODE_TX_RX;
  handle->Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  handle->AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

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
  // handle->Init.ClockSpeed      = WST_I2C_SPEEDCLOCK;
  // handle->Init.DutyCycle       = WST_I2C_DUTYCYCLE;
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
