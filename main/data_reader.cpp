#include "data_reader.hpp"

int step = 0;

void read_gyro() {
  static const char* TAG = "gyro";
  bno055_vector_t v;

  if (gyro_ok != true) {
    gyro_init();
  }
  
  try {
    v = bno->getVectorEuler();
    // ESP_LOGI(TAG, "Euler: X: %.1f Y: %.1f Z: %.1f || Calibration SYS: %u GYRO: %u ACC:%u MAG:%u", v.x, v.y, v.z, cal.sys, cal.gyro, cal.accel, cal.mag);
  }
  catch (BNO055BaseException& ex) {
    ESP_LOGE(TAG, "Something bad happened: %s", ex.what());
    gyro_ok = false;
    return;
  }
  catch (std::exception &ex) {
    gyro_ok = false;
    ESP_LOGE(TAG, "Something bad happened: %s", ex.what());
  }

  storage.pitch = v.y;
  storage.roll = v.z;
  storage.heading = v.x;
}

void read_fake_gyro() {
  double rads = step * 2.0 * PI / 360.0;
  storage.pitch = ((int)(20 *(10.0 * cos(rads + PI / 4.0)))) / 10.0;
  storage.roll = ((int)(30 * (10.0 * sin(rads)))) / 10.0;
  storage.heading = 180 + 180.0 * sin(rads + PI / 3.0);
  storage.altitude = 1000 + 1000 * sin(rads);
  storage.airspeed = 100 + 100 * sin(rads);
  storage.ground_speed = 100 + 90 * sin(rads) - 5;
  storage.desired_heading = 100 + 30 * sin(rads);
  storage.desired_altitude = 1000 + 100 * sin(rads);
  storage.qnh = 1013;
  step++;
}

void gyro_task(void* pvParameters) {
  for (;;) {
    if (gyro_ok == true) {
      read_gyro();
    } else {
      read_fake_gyro();
    }
    
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}
