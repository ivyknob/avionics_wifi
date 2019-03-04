#ifndef MAIN_DATA_READER_H_
#define MAIN_DATA_READER_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "math.h"
#include "esp_timer.h"

#include "gyro.hpp"
#include "main.hpp"

extern void gyro_task(void* pvParameters);

#endif  // MAIN_DATA_READER_H_
