#include "main.hpp"

DataStorage storage;

extern "C" void app_main() {
  // i2c_conf.mode = I2C_MODE_MASTER;
  // i2c_conf.sda_io_num = GPIO_NUM_21;
  // i2c_conf.scl_io_num = GPIO_NUM_22;
  // i2c_conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
  // i2c_conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
  // i2c_conf.master.clk_speed = 100000;
  // i2c_param_config(I2C_NUM_0, &i2c_conf);
  // i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0);
  // i2c_set_timeout(I2C_NUM_0, 30000);
  // // wifi_setup();
  wifi_init_sta();
  ws_server_start();
  gyro_init();
  xTaskCreate(&server_task, "server_task", 3000, NULL, 3, NULL);
  xTaskCreate(&server_handle_task, "server_handle_task", 4000, NULL, 3, NULL);
  xTaskCreate(&websockets_task, "websockets_task", 6000, NULL, 1, NULL);
  xTaskCreate(&gyro_task, "gyro_task", 3000, NULL, 2, NULL);
  // xTaskCreate(&baro_task, "baro_task", 3000, NULL, 2, NULL);
}
