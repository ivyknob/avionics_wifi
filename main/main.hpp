#ifndef MAIN_MAIN_H_
#define MAIN_MAIN_H_
#define PI 3.14159265
#define CAN_PERIOD 100
#define WS_PERIOD 100
#define BLINK_GPIO GPIO_NUM_5

#include <string.h>
#include <stdlib.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/queue.h"
#include "freertos/task.h"

#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"

#include "wifi.hpp"
#include "webserver.hpp"
#include "data_storage.hpp"
#include "demo.hpp"
#include "can_interface.hpp"
#include "status.hpp"

extern DataStorage storage;
// extern Status status;

#endif  // MAIN_MAIN_H_
