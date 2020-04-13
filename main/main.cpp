#include "main.hpp"

DataStorage storage;
// Status status;

void init_system() {
  gpio_pad_select_gpio(BLINK_GPIO);
  gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
  // wifi_setup();
  wifi_init_sta();
  demo_init();
  // gyro_init();
  // gyro init fires gyro_task if everything is ok
  // if not ok, it adds fake gyro
  ws_server_start();
  can_init();
  xTaskCreate(&server_task, "server_task", 3000, NULL, 6, NULL);
  xTaskCreate(&server_handle_task, "server_handle_task", 4000, NULL, 6, NULL);
  xTaskCreate(&websockets_task, "websockets_task", 6000, NULL, 6, NULL);
  xTaskCreate(&status_task, "status_task", 7000, NULL, 6, NULL);
}

void app_main() {
  init_system();
}
