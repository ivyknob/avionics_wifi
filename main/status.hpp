#ifndef MAIN_STATUS_H_
#define MAIN_STATUS_H_

#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <math.h>
#include "main.hpp"

void report();
void set_ws_result(int result);
void set_can_result(int result);
void set_can_init_result(int result);
void set_ws_init_result(int result);
extern void status_task(void* pvParameters);

#endif  // MAIN_STATUS_H_
