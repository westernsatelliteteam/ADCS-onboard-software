#include "uart.h"
#include "board.h"

#include "wst_common.h"

static UART_HandleTypeDef uart_instance;

int uart_init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  WST_UART_CLK_ENABLE();

  WST_UART_TX_GPIO_CLK_ENABLE();
  WST_UART_RX_GPIO_CLK_ENABLE();

  WST_UART_VDD_ENABLE();

  GPIO_InitStruct.Pin       = WST_UART_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = WST_UART_TX_AF;

  HAL_GPIO_Init(WST_UART_TX_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin       = WST_UART_RX_PIN;
  GPIO_InitStruct.Alternate = WST_UART_RX_AF;

  HAL_GPIO_Init(WST_UART_RX_GPIO_PORT, &GPIO_InitStruct);

  uart_instance.Instance = WST_UART;

  uart_instance.Init.BaudRate     = WST_UART_BAUDRATE;
  uart_instance.Init.WordLength   = UART_WORDLENGTH_8B;
  uart_instance.Init.StopBits     = UART_STOPBITS_1;
  uart_instance.Init.Parity       = UART_PARITY_NONE;
  uart_instance.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  uart_instance.Init.Mode         = UART_MODE_TX_RX;
  uart_instance.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  uart_instance.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

  if (HAL_UART_Init(&uart_instance) != HAL_OK) {
    return WST_ERR;
  }
  return WST_OK;
}

int uart_write(uint8_t *data, uint32_t length)
{
  if (HAL_UART_Transmit(&uart_instance, data, length, 0xFFFF) != HAL_OK) {
    return WST_ERR;
  }
  return WST_OK;
}

bool uart_is_writing(void)
{
  return false;
}
