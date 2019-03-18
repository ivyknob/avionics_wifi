#ifndef MAIN_DEMO_H_
#define MAIN_DEMO_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "main.hpp"

extern QueueHandle_t demo_task_queue;
void demo_init();

#endif  // MAIN_DEMO_H_
