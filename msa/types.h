struct travel_type {
  int travel;
  float average_travel;
  int max_travel;
};

struct imu_type {
  char accel_range[20];
  char gyro_range[20];
  char accel_data_rate[20];
  char gyro_data_rate[20];
  float temperature;
  float accel_x;
  float accel_y;
  float accel_z;
  float gyro_x;
  float gyro_y;
  float gyro_z;
  bool error;
};
