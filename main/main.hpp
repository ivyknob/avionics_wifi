#ifndef MAIN_MAIN_H_
#define MAIN_MAIN_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "esp_log.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"

#include "wifi.hpp"
#include "webserver.hpp"
#include "data_storage.hpp"
#include "data_reader.hpp"

#include "gyro.hpp"
// #include "baro.hpp"

// i2c_config_t i2c_conf;
extern DataStorage storage;

#endif  // MAIN_MAIN_H_
