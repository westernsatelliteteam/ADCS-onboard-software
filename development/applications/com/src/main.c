#include "wst_common.h"

#include "com.h"
#include "board.h"
#include "stm32l4xx_it.h"
#include "stm32l4xx_hal.h"
#include "stm32l4xx_nucleo_144.h"

#include "i2c.h"
#include "uart.h"
#include "system_time.h"

#define mainTASK_PRIORITY    (tskIDLE_PRIORITY + 1)

#define TEST_SLAVE_ADDR      0x12
#define TEST_SLAVE_REG_ADDR  0x13

static uint8_t os_started = 0;

static void os_start(void)
{
  os_started = 1;
  vTaskStartScheduler();
}

void delay_us(uint32_t us)
{
  uint32_t start_time = system_time_get();
  while(system_time_cmp_us(start_time, system_time_get()) < us);
}

void delay_ms(uint32_t ms)
{
  uint32_t start_time = system_time_get();
  while(system_time_cmp_ms(start_time, system_time_get()) < ms);
}

void rtos_delay_ms(uint32_t ms)
{
  if (os_started) {
    vTaskDelay(ms);
  }
  else {
    delay_ms(ms);
  }
}

void main_task(void *arg)
{
  uint32_t time = 0;
  char msg[] = "Hello World!\r\n";
  while(1) {
    // i2c_imu_write(TEST_SLAVE_ADDR, TEST_SLAVE_REG_ADDR, 9, (uint8_t*)msg);
    LOG_DEBUG(msg);

    LOG_DEBUG("%u\r\n", system_time_cmp_ms(time, system_time_get()));

    BSP_LED_Toggle(LED_BLUE);

    time = system_time_get();

    rtos_delay_ms(500);
  }
}


/* @brief  System Clock Configuration
*         The system Clock is configured as follows :
*            System Clock source            = PLL (MSI)
*            SYSCLK(Hz)                     = 80000000
*            HCLK(Hz)                       = 80000000
*            AHB Prescaler                  = 1
*            APB1 Prescaler                 = 1
*            APB2 Prescaler                 = 1
*            MSI Frequency(Hz)              = 4000000
*            PLL_M                          = 1
*            PLL_N                          = 40
*            PLL_R                          = 2
*            Flash Latency(WS)              = 4 */
void sysclk_init(void)
{
  /* MSI configuration and activation */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_4);
  LL_RCC_MSI_Enable();
  while(LL_RCC_MSI_IsReady() != 1) 
  {
  };
  
  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_MSI, LL_RCC_PLLM_DIV_1, 40, LL_RCC_PLLR_DIV_2);
  LL_RCC_PLL_Enable();
  LL_RCC_PLL_EnableDomain_SYS();
  while(LL_RCC_PLL_IsReady() != 1) 
  {
  };
  
  /* Sysclk activation on the main PLL */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL) 
  {
  };
  
  /* Set APB1 & APB2 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  /* Set systick to 1ms in using frequency set to 80MHz */
  /* This frequency can be calculated through LL RCC macro */
  /* ex: __LL_RCC_CALC_PLLCLK_FREQ(__LL_RCC_CALC_MSI_FREQ(LL_RCC_MSIRANGESEL_RUN, LL_RCC_MSIRANGE_6), 
                                  LL_RCC_PLLM_DIV_1, 40, LL_RCC_PLLR_DIV_2)*/
  LL_Init1msTick(SYSCLK_FREQ);
  
  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(SYSCLK_FREQ);

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
}

static void board_bringup(void)
{
  HAL_Init();
  sysclk_init();

  uart_init();
  i2c_init();
  system_time_init();
}

int main(void)
{
  int32_t taskStatus;
  board_bringup();

  BSP_LED_Init(LED_BLUE);
  BSP_LED_Init(LED_GREEN);

  LOG_DEBUG("COM Started \r\n");

  taskStatus = xTaskCreate(main_task,
                          "main_task",
                          512,
                          NULL,
                          mainTASK_PRIORITY,
                          NULL);

  RTOS_ERR_CHECK(taskStatus);

  os_start();

  /* Should never reach here */
  while (1);
}

void com_sysTickHandler(void)
{
  if (os_started) {
    xPortSysTickHandler();
  }
}

void error_handler(void)
{
  while (1);
}
