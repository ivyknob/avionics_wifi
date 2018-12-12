#include "main.hpp"

DataStorage storage;

extern "C" void app_main() {
  // wifi_setup();
  wifi_init_sta();
  ws_server_start();
  gyro_init();
  xTaskCreate(&server_task, "server_task", 3000, NULL, 3, NULL);
  xTaskCreate(&server_handle_task, "server_handle_task", 4000, NULL, 3, NULL);
  xTaskCreate(&websockets_task, "websockets_task", 6000, NULL, 1, NULL);
  xTaskCreate(&reader_task, "reader_task", 3000, NULL, 2, NULL);
}
