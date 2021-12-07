#include "sensors.h"

#include "imu.h"
#include "system_time.h"
#include "ADCS.h"

static void sensors_task(void *pvParameters)
{
  float accel_data[3];
  float gyro_data[3];
  
  while(1) {

    imu_get_data(accel_data, gyro_data);

    LOG_DEBUG("gyro: %7.4f, %7.4f, %7.4f accel: %7.4f, %7.4f, %7.4f\r\n",
            gyro_data[0],
            gyro_data[1],
            gyro_data[2],
            accel_data[0],
            accel_data[1],
            accel_data[2]);

    rtos_delay_ms(50);
  }
}

void sensors_task_setup(void)
{
  imu_init();

  LOG_DEBUG("Sensors Initialized\r\n");
}

void sensors_task_start(void)
{
  BaseType_t task_status = xTaskCreate(sensors_task,
                                       "sensors_task",
                                       SENSORS_STACK_SIZE,
                                       NULL,
                                       sensors_TASK_PRIORITY,
                                       NULL);

  RTOS_ERR_CHECK(task_status);
}