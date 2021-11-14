#ifndef BOARD_H
#define BOARD_H

#include "stm32l4xx_hal.h"
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_pwr.h"
#include "stm32l4xx_ll_rcc.h"
#include "stm32l4xx_ll_system.h"
#include "stm32l4xx_ll_tim.h"
#include "stm32l4xx_ll_utils.h"

#define SYSCLK_FREQ                           80000000

/* System Time */

#define SYSTEM_TIME_TIMER         TIM2
#define SYSTEM_TIME_CLK_EN()      LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2)

/* UART */

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

/*I2C*/

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


#endif // BOARD_H