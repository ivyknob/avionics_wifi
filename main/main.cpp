#include "main.hpp"

DataStorage storage;

void init_system() {
  // wifi_setup();
  wifi_init_sta();
  gyro_init();
  // gyro init fires gyro_task if everything is ok
  // if not ok, it adds fake gyro 
  ws_server_start();
  // can_init();
  xTaskCreate(&server_task, "server_task", 3000, NULL, 6, NULL);
  xTaskCreate(&server_handle_task, "server_handle_task", 4000, NULL, 6, NULL);
  xTaskCreate(&websockets_task, "websockets_task", 6000, NULL, 6, NULL);
}

extern "C" void app_main() {
  init_system();
}
