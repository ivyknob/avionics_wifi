#ifndef MAIN_WEBSERVER_H_
#define MAIN_WEBSERVER_H_


#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/queue.h"
#include <math.h>

#include "esp_log.h"
#include "esp_event.h"
#include "esp_system.h"
#include "nvs_flash.h"

#include "cJSON.h"

// #include "lwip/err.h"
// #include "lwip/sockets.h"
// #include "lwip/sys.h"
// #include "lwip/netdb.h"
// #include "lwip/dns.h"

#include "websocket_server.h"
#include "main.hpp"

void websocket_callback(uint8_t num, WEBSOCKET_TYPE_t type, char* msg, uint64_t len);
void http_serve(struct netconn *conn);
extern void server_task(void* pvParameters);
extern void server_handle_task(void* pvParameters);
extern void websockets_task(void* pvParameters);

#endif  // MAIN_WEBSERVER_H_
