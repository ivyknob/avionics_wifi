#ifndef MAIN_DATA_STORAGE_H_
#define MAIN_DATA_STORAGE_H_

class DataStorage {
  public:
    double pitch;
    double roll;
    double heading;
    double vsi;
    double qnh;
    int altitude;
    int airspeed;
    int ground_speed;
    int desired_altitude;
    int desired_heading;
};

#endif  // MAIN_DATA_STORAGE_H_
