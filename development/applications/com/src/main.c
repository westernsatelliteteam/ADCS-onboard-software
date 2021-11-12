#include "bsp.h"
#include "com.h"
#include "stm32l4xx_it.h"
#include "stm32l4xx_hal.h"
#include "stm32l4xx_nucleo_144.h"

// #include "i2c.h"

#define mainTASK_PRIORITY    (tskIDLE_PRIORITY + 1)

#define TEST_SLAVE_ADDR      0x12
#define TEST_SLAVE_REG_ADDR  0x13

static char tx_data[10] = "i2c_test\n";

void main_task(void *arg)
{
  char msg[] = "Hello World!\r\n";
  while(1) {
    // i2c_write(TEST_SLAVE_ADDR, TEST_SLAVE_REG_ADDR, 9, (uint8_t*)tx_data);
    DEBUG_LOG("Sent\r\n");

    BSP_LED_Toggle(LED_BLUE);

    vTaskDelay(500);
  }
}

int main(void)
{
  int32_t taskStatus;
  bsp_board_bringup();

  BSP_LED_Init(LED_BLUE);
  BSP_LED_Init(LED_GREEN);
  logger_init();
  // i2c_init();

  DEBUG_LOG("COM Started \r\n");

  taskStatus = xTaskCreate(main_task,
                          "main_task",
                          512,
                          NULL,
                          mainTASK_PRIORITY,
                          NULL);

  RTOS_ERR_CHECK(taskStatus);

  OSStarted();

  vTaskStartScheduler();

  /* Should never reach here */
  while (1);
}
