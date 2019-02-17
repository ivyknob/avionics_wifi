#ifndef CAN_INTERFACE_H_
#define CAN_INTERFACE_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "esp_log.h"
#include "esp_event_loop.h"

#include "driver/gpio.h"
#include "driver/can.h"

void can_init();

extern void can_transmit_task(void* pvParameters);
extern void can_receive_task(void* pvParameters);
extern void can_control_task(void* pvParameters);

#endif
