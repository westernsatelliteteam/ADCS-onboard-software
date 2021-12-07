#include "wst_common.h"

#include "com.h"
#include "board.h"
#include "stm32l4xx_it.h"
#include "stm32l4xx_hal.h"
#include "stm32l4xx_nucleo_144.h"

#include "i2c.h"
#include "uart.h"
#include "system_time.h"
#include "icm20948_api.h"

#define mainTASK_PRIORITY    (tskIDLE_PRIORITY + 1)

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

icm20948_return_code_t usr_write(uint8_t addr, uint8_t *data, uint32_t len) {
  icm20948_return_code_t ret = ICM20948_RET_OK;
  i2c_imu_write(0x68, (uint8_t)addr, data, (uint16_t)len);
  return ret;
}

icm20948_return_code_t usr_read(uint8_t addr, uint8_t *data, uint32_t len) {
  icm20948_return_code_t ret = ICM20948_RET_OK;
  i2c_imu_read(0x68, (uint8_t)addr, data, (uint16_t)len);
  return ret;
}

void usr_delay_us(uint32_t period) {
  delay_us(period);
}


void main_task(void *arg)
{
  uint32_t time = 0;
  char msg[] = "Hello World!\r\n";

  icm20948_return_code_t ret = ICM20948_RET_OK;
  icm20948_settings_t settings;
  icm20948_gyro_t gyro_data;
  icm20948_accel_t accel_data;

  // Init the device function pointers
  ret = icm20948_init(usr_read, usr_write, usr_delay_us);

  // Check if we successfully stored the function poiners provided
  if( ret == ICM20948_RET_OK ) {
      // Enable the Gyro
      settings.gyro.en = ICM20948_MOD_ENABLED;
      // Select the +-20000dps range
      settings.gyro.fs = ICM20948_GYRO_FS_SEL_2000DPS;
      // Enable the Accel
      settings.accel.en = ICM20948_MOD_ENABLED;
      // Select the +-2G range
      settings.accel.fs = ICM20948_ACCEL_FS_SEL_2G;
      ret = icm20948_applySettings(&settings);
  }

  while(1) {
    // LOG_DEBUG(msg);

    // LOG_DEBUG("%u\r\n", system_time_cmp_ms(time, system_time_get()));

    BSP_LED_Toggle(LED_BLUE);

    time = system_time_get();

    // Retrieve the Gyro data and store it in our gyro_data struct
    // Output is in dps (Degress per second)
    ret |= icm20948_getGyroData(&gyro_data);
    // Retrieve the Accel data and store it in our accel_data struct
    // Output is in mG
    ret |= icm20948_getAccelData(&accel_data);

    LOG_DEBUG("gyro: %d, %d, %d accel: %d, %d, %d\r\n",
            gyro_data.x,
            gyro_data.y,
            gyro_data.z,
            accel_data.x,
            accel_data.y,
            accel_data.z);

    rtos_delay_ms(50);
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
  BSP_LED_Init(LED_RED);

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
