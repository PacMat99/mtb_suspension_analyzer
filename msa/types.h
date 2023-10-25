struct travel_type {
    int travel;
    float average_travel;
    int max_travel;
};
typedef struct travel_type ttravel_type;

struct imu_type {
    float temperature;
    float accel_x;
    float accel_y;
    float accel_z;
    float gyro_x;
    float gyro_y;
    float gyro_z;
};
typedef struct imu_type timu_type;