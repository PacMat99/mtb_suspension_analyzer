struct tof_type {
  int travel;
  uint16_t reading_n;
  float average_travel;
  int max_travel;
  bool error;
};

void tof_setup(void);
//tof_type tof_loop(bool recording);
float tof_loop(bool recording);
