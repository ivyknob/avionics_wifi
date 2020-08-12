#ifndef MAIN_WIFI_H_
#define MAIN_WIFI_H_

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

void wifi_init_sta(void);

// static esp_err_t event_handler(void* ctx, system_event_t* event)
// static void wifi_setup()

#endif  // MAIN_WIFI_H_
