#include "demo.hpp"

int step = 0;
int step_direction = 1;
QueueHandle_t demo_task_queue;

void fire_demo_step() {
  double roll_k = 0.05;
  double rads = step * 2.0 * PI / 360.0;
  storage.pitch = 20.0 * cos(rads + PI / 4.0);
  storage.roll = 30 * sin(rads);
  storage.heading = storage.heading - storage.roll * roll_k ;
  if (storage.heading > 360.0) { storage.heading = storage.heading - 360.0; }
  if (storage.heading < 0.0) { storage.heading = storage.heading + 360.0 ; }
  storage.altitude = storage.altitude + storage.vsi / 20.0;
  storage.airspeed = 150 + 30 * sin(rads);
  storage.ground_speed = abs(storage.airspeed * cos(storage.pitch * 2.0 * PI / 360.0));
  storage.desired_heading = 230;
  storage.desired_altitude = 1000;
  storage.vsi = storage.airspeed * sin(storage.pitch * 2.0 * PI / 360.0);
  storage.qnh = 1013.1;

  if (step >= 1000 || step < 0) {
    storage.altitude = 1000;
    step_direction = (-1) * step_direction;
  }
  step += step_direction;
}

void demo_task(void* pvParameters) {
  int demo_period = 50;
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
  storage.altitude = 1000;
  demo_task_queue = xQueueCreate(1, sizeof(int));
  xTaskCreate(&demo_task, "demo_task", 3000, NULL, 6, NULL);
}
