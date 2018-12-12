#include "data_reader.hpp"

void read_data() {
  static const char* TAG = "data_reader";
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

  storage.pitch = v.z;
  storage.roll = v.y;
  storage.heading = v.x;
}

void reader_task(void* pvParameters) {
  for (;;) {
    read_data();
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}
