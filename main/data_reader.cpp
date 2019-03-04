#include "data_reader.hpp"

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
  unsigned long ts = int(esp_timer_get_time() / 10000);
  ts = ts % 360;
  double rads = ts * 2.0 * PI / 360.0;
  storage.pitch = 10.0 * cos(rads + PI / 4.0);
  storage.roll = 10.0 * sin(rads);
  storage.heading = 180 + 180.0 * sin(rads + PI / 3.0);
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
