#include "status.hpp"

int can_init_result;
int can_last_result;
uint32_t can_last_millis;
int ws_init_result;
int ws_last_result;
uint32_t ws_last_millis;

uint32_t millis() {
  return xTaskGetTickCount() * portTICK_PERIOD_MS;
}

void set_can_result(int result) {
  can_last_result = result;
  can_last_millis = millis();
}

void set_ws_result(int result) {
  ws_last_result = result;
  ws_last_millis = millis();
}

void blink() {
  gpio_set_level(BLINK_GPIO, 1);
  vTaskDelay(100 / portTICK_PERIOD_MS);
  gpio_set_level(BLINK_GPIO, 0);
}

bool is_ws_ok() {
  if (ws_last_result != 0) { return false; }
  return (millis() - ws_last_millis) < 3 * WS_PERIOD;
}

bool is_can_ok() {
  if (can_last_result != 0) { return false; }
  return (millis() - can_last_millis) < 3 * CAN_PERIOD;
}

bool is_ok() {
  return (is_can_ok() && is_ws_ok());
}

void report() {
  if (is_ok()) {
    blink();
  }
  if (!is_can_ok()) {
    blink();
    blink();
    ESP_LOGE("STATUS", "Can error");
  }
  // if (!is_ws_ok()) { blink(); blink(); blink(); ESP_LOGE("STATUS", "WS Error"); return; }
}

void status_task(void* pvParameters) {
  for(;;){
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    report();
  }
}
