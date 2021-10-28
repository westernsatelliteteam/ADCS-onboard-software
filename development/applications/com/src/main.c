#include "bsp.h"
#include "com.h"
#include "stm32f4xx_it.h"

#include "logger.h"
#include "i2c.h"

#include <stdint.h>

#define mainTASK_PRIORITY    (tskIDLE_PRIORITY + 1)

#define TEST_SLAVE_ADDR      0x12
#define TEST_SLAVE_REG_ADDR  0x13

static char tx_data[10] = "i2c_test\n";

void main_task(void *arg)
{
  while(1) {
    i2c_write(TEST_SLAVE_ADDR, TEST_SLAVE_REG_ADDR, 9, (uint8_t*)tx_data);
    DEBUG_LOG("Sent\r\n");
    vTaskDelay(100);
  }
}

int main(void)
{
  int32_t taskStatus;
  bsp_board_bringup();
  logger_init();
  i2c_init();

  DEBUG_LOG("COM Started \r\n");

  taskStatus = xTaskCreate(main_task,
                          "main_task",
                          128,
                          NULL,
                          mainTASK_PRIORITY,
                          NULL);

  RTOS_ERR_CHECK(taskStatus);

  OSStarted();

  vTaskStartScheduler();

  /* Should never reach here */
  while (1);
}
