#ifndef MAIN_WIFI_H_
#define MAIN_WIFI_H_

#include "lwip/api.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"

#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event.h"
#include "nvs_flash.h"

#define AP_SSID CONFIG_AP_SSID
#define AP_PSSWD CONFIG_AP_PSSWD

esp_err_t event_handler(void *ctx, system_event_t *event);
void wifi_init_sta();

// static esp_err_t event_handler(void* ctx, system_event_t* event)
// static void wifi_setup()

#endif  // MAIN_WIFI_H_
