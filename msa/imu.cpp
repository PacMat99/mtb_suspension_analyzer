#include <Arduino.h>
#include <string>
// Basic demo for accelerometer & gyro readings from Adafruit
// LSM6DSOX sensor
#include <Adafruit_LSM6DSOX.h>
#include "imu.h"

Adafruit_LSM6DSOX sox;

imu_type imu;

void imu_setup(void) {
  while (!sox.begin_I2C()) {
    Serial.println("LSM6DSOX error");
    delay(500);
  }

  // sox.setAccelRange(LSM6DS_ACCEL_RANGE_2_G);
  Serial.print("Accelerometer range set to: ");
  switch (sox.getAccelRange()) {
    case LSM6DS_ACCEL_RANGE_2_G:
      strcpy(imu.accel_range, "+-2G");
      break;
    case LSM6DS_ACCEL_RANGE_4_G:
      strcpy(imu.accel_range, "+-4G");
      break;
    case LSM6DS_ACCEL_RANGE_8_G:
      strcpy(imu.accel_range, "+-8G");
      break;
    case LSM6DS_ACCEL_RANGE_16_G:
      strcpy(imu.accel_range, "+-16G");
      break;
  }
  Serial.println(imu.accel_range);

  // sox.setGyroRange(LSM6DS_GYRO_RANGE_250_DPS );
  Serial.print("Gyro range set to: ");
  switch (sox.getGyroRange()) {
    case LSM6DS_GYRO_RANGE_125_DPS:
      strcpy(imu.gyro_range, "125 degrees/s");
      break;
    case LSM6DS_GYRO_RANGE_250_DPS:
      strcpy(imu.gyro_range, "250 degrees/s");
      break;
    case LSM6DS_GYRO_RANGE_500_DPS:
      strcpy(imu.gyro_range, "500 degrees/s");
      break;
    case LSM6DS_GYRO_RANGE_1000_DPS:
      strcpy(imu.gyro_range, "1000 degrees/s");
      break;
    case LSM6DS_GYRO_RANGE_2000_DPS:
      strcpy(imu.gyro_range, "2000 degrees/s");
      break;
  }
  Serial.println(imu.gyro_range);

  // sox.setAccelDataRate(LSM6DS_RATE_12_5_HZ);
  Serial.print("Accelerometer data rate set to: ");
  switch (sox.getAccelDataRate()) {
    case LSM6DS_RATE_SHUTDOWN:
      strcpy(imu.accel_data_rate, "0 Hz");
      break;
    case LSM6DS_RATE_12_5_HZ:
      strcpy(imu.accel_data_rate, "12.5 Hz");
      break;
    case LSM6DS_RATE_26_HZ:
      strcpy(imu.accel_data_rate, "26 Hz");
      break;
    case LSM6DS_RATE_52_HZ:
      strcpy(imu.accel_data_rate, "52 Hz");
      break;
    case LSM6DS_RATE_104_HZ:
      strcpy(imu.accel_data_rate, "104 Hz");
      break;
    case LSM6DS_RATE_208_HZ:
      strcpy(imu.accel_data_rate, "208 Hz");
      break;
    case LSM6DS_RATE_416_HZ:
      strcpy(imu.accel_data_rate, "416 Hz");
      break;
    case LSM6DS_RATE_833_HZ:
      strcpy(imu.accel_data_rate, "833 Hz");
      break;
    case LSM6DS_RATE_1_66K_HZ:
      strcpy(imu.accel_data_rate, "1.66 KHz");
      break;
    case LSM6DS_RATE_3_33K_HZ:
      strcpy(imu.accel_data_rate, "3.33 KHz");
      break;
    case LSM6DS_RATE_6_66K_HZ:
      strcpy(imu.accel_data_rate, "6.66 KHz");
      break;
  }
  Serial.println(imu.accel_data_rate);

  // sox.setGyroDataRate(LSM6DS_RATE_12_5_HZ);
  Serial.print("Gyro data rate set to: ");
  switch (sox.getGyroDataRate()) {
    case LSM6DS_RATE_SHUTDOWN:
      strcpy(imu.gyro_data_rate, "0 Hz");
      break;
    case LSM6DS_RATE_12_5_HZ:
      strcpy(imu.gyro_data_rate, "12.5 Hz");
      break;
    case LSM6DS_RATE_26_HZ:
      strcpy(imu.gyro_data_rate, "26 Hz");
      break;
    case LSM6DS_RATE_52_HZ:
      strcpy(imu.gyro_data_rate, "52 Hz");
      break;
    case LSM6DS_RATE_104_HZ:
      strcpy(imu.gyro_data_rate, "104 Hz");
      break;
    case LSM6DS_RATE_208_HZ:
      strcpy(imu.gyro_data_rate, "208 Hz");
      break;
    case LSM6DS_RATE_416_HZ:
      strcpy(imu.gyro_data_rate, "416 Hz");
      break;
    case LSM6DS_RATE_833_HZ:
      strcpy(imu.gyro_data_rate, "833 Hz");
      break;
    case LSM6DS_RATE_1_66K_HZ:
      strcpy(imu.gyro_data_rate, "1.66 KHz");
      break;
    case LSM6DS_RATE_3_33K_HZ:
      strcpy(imu.gyro_data_rate, "3.33 KHz");
      break;
    case LSM6DS_RATE_6_66K_HZ:
      strcpy(imu.gyro_data_rate, "6.66 KHz");
      break;
  }
  Serial.println(imu.gyro_data_rate);
}

imu_type imu_loop(bool recording) {
  //  /* Get a new normalized sensor event */
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;
  sox.getEvent(&accel, &gyro, &temp);

  imu.temperature = temp.temperature;

  Serial.print("\t\tTemperature ");
  Serial.print(imu.temperature);
  Serial.println(" deg C");

  /* Display the results (acceleration is measured in m/s^2) */
  imu.accel_x = accel.acceleration.x;
  imu.accel_y = accel.acceleration.y;
  imu.accel_z = accel.acceleration.z;
  Serial.print("\t\tAccel X: ");
  Serial.print(imu.accel_x);
  Serial.print(" \tY: ");
  Serial.print(imu.accel_y);
  Serial.print(" \tZ: ");
  Serial.print(imu.accel_z);
  Serial.println(" m/s^2 ");

  /* Display the results (rotation is measured in rad/s) */
  imu.gyro_x = gyro.gyro.x;
  imu.gyro_y = gyro.gyro.y;
  imu.gyro_z = gyro.gyro.z;
  Serial.print("\t\tGyro X: ");
  Serial.print(imu.gyro_x);
  Serial.print(" \tY: ");
  Serial.print(imu.gyro_y);
  Serial.print(" \tZ: ");
  Serial.print(imu.gyro_z);
  Serial.println(" radians/s ");
  Serial.println();

  delay(100);

  // serial plotter friendly format

  //Serial.print(temp.temperature);
  //Serial.print(",");

  //Serial.print(accel.acceleration.x);
  //Serial.print(","); Serial.print(accel.acceleration.y);
  //Serial.print(","); Serial.print(accel.acceleration.z);
  //Serial.print(",");

  //Serial.print(gyro.gyro.x);
  //Serial.print(","); Serial.print(gyro.gyro.y);
  //Serial.print(","); Serial.print(gyro.gyro.z);
  //Serial.println();
  //delayMicroseconds(10000);

  Serial.print("SDA: ");
  Serial.println(SDA);
  Serial.print("SCL: ");
  Serial.println(SCL);

  imu.error = false;

  return imu;
}
