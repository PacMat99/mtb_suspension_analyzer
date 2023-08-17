struct travel_type {
  int travel;
  uint16_t reading_n;
  float average_travel;
  int max_travel;
  bool error;
};

void travel_setup(void);
//travel_type travel_loop(bool recording);
uint16_t travel_loop(bool recording);
