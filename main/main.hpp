#ifndef MAIN_MAIN_H_
#define MAIN_MAIN_H_
#define PI 3.14159265

#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/queue.h"
#include "freertos/task.h"

#include "esp_event_loop.h"
#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"

#include "wifi.hpp"
#include "webserver.hpp"
#include "data_storage.hpp"
#include "data_reader.hpp"

#include "gyro.hpp"
#include "can_interface.hpp"

extern DataStorage storage;

#endif  // MAIN_MAIN_H_
