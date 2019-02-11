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

  storage.pitch = v.z;
  storage.roll = v.y;
  storage.heading = v.x;
}

// void baro_task(void* pvParameters) {
//   static const char* TAG = "baro";
//   bme280_reading_data sensor_data = bme280.readSensorData();

//   printf("Temperature: %.2foC, Humidity: %.2f%%, Pressure: %.2fPa\n",
//           (double) sensor_data.temperature,
//           (double) sensor_data.humidity,
//           (double) sensor_data.pressure
//   );
//   vTaskDelay(1000 / portTICK_PERIOD_MS);
// }

void gyro_task(void* pvParameters) {
  for (;;) {
    read_gyro();
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}
