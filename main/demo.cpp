#include "demo.hpp"

int step = 0;
int step_direction = 1;
QueueHandle_t demo_task_queue;

void fire_demo_step() {
  double rads = step * 2.0 * PI / 360.0;
  storage.pitch = 20.0 * cos(rads + PI / 4.0);
  storage.roll = 30 * sin(rads);
  storage.heading = 180 + 180.0 * sin(rads + PI / 3.0);
  storage.altitude = 1000 + 1000 * sin(rads);
  storage.airspeed = 100 + 100 * sin(rads);
  storage.ground_speed = 100 + 90 * sin(rads) - 5;
  storage.desired_heading = 100 + 30 * sin(rads);
  storage.desired_altitude = 1000 + 100 * sin(rads);
  storage.vsi = 10 * sin(rads / 3.0);
  storage.qnh = 1013;

  step += step_direction;
  if (step >= 1000 || step <= 0) {
    step_direction = (-1) * step_direction;
  }
}

void demo_task(void* pvParameters) {
  int demo_period = 100;
  int enabled = 0;
  for(;;) {
    xQueueReceive(demo_task_queue, &enabled, demo_period / 10);
    if (enabled > 0) {
      fire_demo_step();
    }
    vTaskDelay(demo_period / portTICK_PERIOD_MS);
  }
}

void demo_init() {
  demo_task_queue = xQueueCreate(1, sizeof(int));
  xTaskCreate(&demo_task, "demo_task", 3000, NULL, 6, NULL);
}
