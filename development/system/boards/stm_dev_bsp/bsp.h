#ifndef BSP_H
#define BSP_H

#include <stdint.h>

#ifdef BOARD_NUCLEO
// #include "stm32l4xx_nucleo_144.h"
#include "stm32l4xx_hal.h"

#endif

#include "wst_common.h"

void bsp_board_bringup(void);

/* UART */
typedef UART_HandleTypeDef wst_uart_handle_t;

#define WST_UART                         LPUART1
#define WST_UART_CLK_ENABLE()            __HAL_RCC_LPUART1_CLK_ENABLE()
#define WST_UART_RX_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOG_CLK_ENABLE()
#define WST_UART_TX_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOG_CLK_ENABLE()
#define WST_UART_VDD_ENABLE()              HAL_PWREx_EnableVddIO2()

#define WST_UART_FORCE_RESET()           __HAL_RCC_LPUART1_FORCE_RESET()
#define WST_UART_RELEASE_RESET()         __HAL_RCC_LPUART1_RELEASE_RESET()

#define WST_UART_TX_PIN                  GPIO_PIN_7
#define WST_UART_TX_GPIO_PORT            GPIOG
#define WST_UART_TX_AF                   GPIO_AF8_LPUART1
#define WST_UART_RX_PIN                  GPIO_PIN_8
#define WST_UART_RX_GPIO_PORT            GPIOG
#define WST_UART_RX_AF                   GPIO_AF8_LPUART1

#define WST_UART_BAUDRATE                115200

int  bsp_uart_init(wst_uart_handle_t *handle);
void bsp_uart_put_char(wst_uart_handle_t *handle, uint8_t *ch);
void bsp_uart_write(wst_uart_handle_t *handle, uint8_t *data, uint16_t length);

/*I2C*/
typedef I2C_HandleTypeDef wst_i2c_handle_t;

#define WST_I2C                            I2C2
#define WST_I2C_CLK_ENABLE()               __HAL_RCC_I2C2_CLK_ENABLE()
#define WST_I2C_SDA_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOF_CLK_ENABLE()
#define WST_I2C_SCL_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOF_CLK_ENABLE()

#define WST_I2C_FORCE_RESET()              __HAL_RCC_I2C2_FORCE_RESET()
#define WST_I2C_RELEASE_RESET()            __HAL_RCC_I2C2_RELEASE_RESET()

#define WST_I2C_SCL_PIN                            GPIO_PIN_1
#define WST_I2C_SCL_GPIO_PORT                      GPIOF
#define WST_I2C_SDA_PIN                            GPIO_PIN_0
#define WST_I2C_SDA_GPIO_PORT                      GPIOF
#define WST_I2C_SCL_SDA_AF                         GPIO_AF4_I2C2

#define WST_I2C_SPEEDCLOCK                         400000
#define WST_I2C_DUTYCYCLE                          I2C_DUTYCYCLE_2

int bsp_i2c_init(wst_i2c_handle_t *handle);

int bsp_i2c_write(wst_i2c_handle_t *handle,
                  uint8_t slave_addr,
                  uint8_t reg_addr,
                  uint16_t length,
                  uint8_t *data);

int bsp_i2c_read(wst_i2c_handle_t *handle,
                 uint8_t slave_addr,
                 uint8_t reg_addr,
                 uint16_t length,
                 uint8_t *data);

#endif  // BSP_H
