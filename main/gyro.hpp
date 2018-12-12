#ifndef MAIN_GYRO_H_
#define MAIN_GYRO_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include <math.h>
#include "BNO055ESP32.h"

void gyro_init();

extern BNO055* bno;
extern bool gyro_ok;

#endif  // MAIN_GYRO_H_