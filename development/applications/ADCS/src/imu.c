#include "imu.h"

#include "ADCS.h"
#include "i2c.h"
#include "icm20948_api.h"

icm20948_return_code_t usr_write(uint8_t addr, const uint8_t *data, uint32_t len) {
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


int imu_init(void)
{
  icm20948_return_code_t ret = ICM20948_RET_OK;
  icm20948_settings_t settings;

  // Init the device function pointers
  ret = icm20948_init(usr_read, usr_write, usr_delay_us);

  // Check if we successfully stored the function poiners provided
  if(ret == ICM20948_RET_OK) {
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

  return ret;
}

int imu_get_data(float *accel, float *gyro)
{
  icm20948_gyro_t gyro_data;
  icm20948_accel_t accel_data;
  icm20948_return_code_t ret = ICM20948_RET_OK;

  // Retrieve the Gyro data and store it in our gyro_data struct
  // Output is in dps (Degress per second)
  ret |= icm20948_getGyroData(&gyro_data);
  // Retrieve the Accel data and store it in our accel_data struct
  // Output is in mG
  ret |= icm20948_getAccelData(&accel_data);

  gyro[0] = (float)gyro_data.x;
  gyro[1] = (float)gyro_data.y;
  gyro[2] = (float)gyro_data.z;
  accel[0] = (float)accel_data.x*(9.81f/1000);
  accel[1] = (float)accel_data.y*(9.81f/1000);
  accel[2] = (float)accel_data.z*(9.81f/1000);

  return ret;
}