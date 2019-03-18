#include "can_interface.hpp"

#define NO_OF_MSGS              10
#define NO_OF_ITERS             3
#define TX_GPIO_NUM             21
#define RX_GPIO_NUM             22
#define TX_TASK_PRIO            8       //Sending task priority
#define RX_TASK_PRIO            9       //Receiving task priority
#define CTRL_TSK_PRIO           10      //Control task priority
#define LEVEL_ID                0x321   //11 bit standard format ID
#define AIR_ID                  0x40   //11 bit standard format ID
#define GPS_ID                  0x100   //11 bit standard format ID


static QueueHandle_t can_rx_task_queue;
static QueueHandle_t can_tx_task_queue;

// SemaphoreHandle_t tx_sem;
// SemaphoreHandle_t rx_sem;
// SemaphoreHandle_t ctrl_sem;
// SemaphoreHandle_t done_sem;

void can_init() {
  static const char* TAG = "CAN Init";
  can_rx_task_queue = xQueueCreate(10, sizeof(can_message_t));
  can_tx_task_queue = xQueueCreate(10, sizeof(can_message_t));
//   return;
//   tx_sem = xSemaphoreCreateBinary();
  // rx_sem = xSemaphoreCreateBinary();
//   ctrl_sem = xSemaphoreCreateBinary();
//   done_sem = xSemaphoreCreateBinary();
  can_general_config_t g_config = CAN_GENERAL_CONFIG_DEFAULT(GPIO_NUM_21, GPIO_NUM_22, CAN_MODE_NORMAL);
  // can_timing_config_t t_config = CAN_TIMING_CONFIG_500KBITS();
  // can_filter_config_t f_config = CAN_FILTER_CONFIG_ACCEPT_ALL();

  can_timing_config_t t_config = CAN_TIMING_CONFIG_1MBITS();
  // can_timing_config_t t_config = {.brp = 4, .tseg_1 = 15, .tseg_2 = 3, .sjw = 1, .triple_sampling = false};
  can_filter_config_t f_config = CAN_FILTER_CONFIG_ACCEPT_ALL();
  // can_filter_config_t f_config = {.acceptance_code = (MSG_ID << 21),
  //                                 .acceptance_mask = ~(CAN_STD_ID_MASK << 21),
  //                                 .single_filter = (bool)true};
  // can_general_config_t g_config = CAN_GENERAL_CONFIG_DEFAULT(GPIO_NUM_21, GPIO_NUM_22, CAN_MODE_NO_ACK);
  // can_general_config_t g_config = CAN_GENERAL_CONFIG_DEFAULT(GPIO_NUM_21, GPIO_NUM_22, CAN_MODE_LISTEN_ONLY);


  if (can_driver_install(&g_config, &t_config, &f_config) == ESP_OK) {
    ESP_LOGI(TAG, "CAN Driver installed");
  } else {
    ESP_LOGE(TAG, "Failed to install CAN Driver");
    return;
  }

  if (can_start() == ESP_OK) {
    ESP_LOGI(TAG, "CAN Driver started");
  } else {
    ESP_LOGE(TAG, "Unable to start CAN Driver");
    return;
  }

  uint32_t alerts_to_enable = CAN_ALERT_ALL | CAN_ALERT_AND_LOG;
  if (can_reconfigure_alerts(alerts_to_enable, NULL) == ESP_OK) {
      printf("Alerts reconfigured\n");
  } else {
      printf("Failed to reconfigure alerts");
  }

//   xTaskCreatePinnedToCore(can_control_task, "CAN_ctrl", 4096, NULL, CTRL_TSK_PRIO, NULL, tskNO_AFFINITY);
//   xTaskCreatePinnedToCore(can_receive_task, "CAN_rx", 4096, NULL, RX_TASK_PRIO, NULL, tskNO_AFFINITY);
//   xTaskCreatePinnedToCore(can_transmit_task, "CAN_tx", 4096, NULL, TX_TASK_PRIO, NULL, tskNO_AFFINITY);
  ESP_LOGI(TAG, "before CAN_rx task");
  xTaskCreatePinnedToCore(can_receive_task, "CAN_RX", 10000, NULL, RX_TASK_PRIO, NULL, tskNO_AFFINITY);
  xTaskCreate(&can_rx_parse_task, "CAN_RX_PARSE", 10000, NULL, 10, NULL);
  // xTaskCreate(&can_receive_task, "CAN_rx", 10000, NULL, 1, NULL);
  ESP_LOGI(TAG, "After CAN_rx task");

  //Install CAN driver
//   ESP_ERROR_CHECK(can_driver_install(&g_config, &t_config, &f_config));
//   ESP_LOGI(TAG, "Driver installed");

  //Start control task
//   xSemaphoreGive(ctrl_sem);
  //Wait for all iterations and tasks to complete running
//   xSemaphoreTake(done_sem, portMAX_DELAY);

  //Uninstall CAN driver
//   ESP_ERROR_CHECK(can_driver_uninstall());
//   ESP_LOGI(TAG, "Driver uninstalled");

  //Cleanup
//   vSemaphoreDelete(tx_sem);
//   vSemaphoreDelete(rx_sem);
//   vSemaphoreDelete(ctrl_sem);
//   vQueueDelete(done_sem);
}

// void can_transmit_task(void* pvParameters) {
//     can_message_t tx_msg = {.flags = CAN_MSG_FLAG_SELF, .identifier = MSG_ID, .data_length_code = 1};
//     for (int iter = 0; iter < NO_OF_ITERS; iter++) {
//         xSemaphoreTake(tx_sem, portMAX_DELAY);
//         for (int i = 0; i < NO_OF_MSGS; i++) {
//             //Transmit messages using self reception request
//             tx_msg.data[0] = i;
//             ESP_ERROR_CHECK(can_transmit(&tx_msg, portMAX_DELAY));
//             vTaskDelay(pdMS_TO_TICKS(10));
//         }
//     }
//     vTaskDelete(NULL);
// }

void can_receive_task(void* pvParameters) {
    static const char* TAG = "CAN RX";
    ESP_LOGI(TAG, "Starting CAN_rx task");
    int demo_enabled = 0;
    int at_least_one_message_received = 0;
    for (;;){
        bool failed = true;
        can_message_t rx_message;
        esp_err_t res = can_receive(&rx_message, pdMS_TO_TICKS(10000));

        switch(res) {
          case ESP_OK:
            // printf("Message received\n");
            failed = false;
            break; //optional
          case ESP_ERR_TIMEOUT:
            ESP_LOGE(TAG, "Failed to receive message due to timeout");
            failed = true;
            break; //optional
          case ESP_ERR_INVALID_ARG:
            ESP_LOGE(TAG, "Failed to receive message due to invalid arguments");
            failed = true;
            break; //optional
          case ESP_ERR_INVALID_STATE:
            ESP_LOGE(TAG, "Failed to receive message due to driver invalid state\n");
            failed = true;
            break; //optional     
        }

        if (failed == false) {
          xQueueSend(can_rx_task_queue, &rx_message, portMAX_DELAY);
          set_can_result(0);
          at_least_one_message_received = 1;
          if (demo_enabled > 0){
            demo_enabled = 0;
            xQueueSend(demo_task_queue, &demo_enabled, portMAX_DELAY);
          }
        } else {
          set_can_result(1);
          if (demo_enabled == 0 && at_least_one_message_received == 0){
            demo_enabled = 1;
            xQueueSend(demo_task_queue, &demo_enabled, portMAX_DELAY);
          }
        }
        vTaskDelay(20 / portTICK_PERIOD_MS);
    }
    // vTaskDelete(NULL);
}

void can_rx_parse_task(void* pvParameters) {
  can_message_t message;
  union {
    char conv_buf[8];
    short two_byte_value;
    int four_byte_value;
  };

  for(;;) {
    xQueueReceive(can_rx_task_queue, &message, portMAX_DELAY);
    if (!(message.flags & CAN_MSG_FLAG_RTR)) {
      switch (message.identifier) {
        case AIR_ID:
          four_byte_value = 0;
          memcpy(conv_buf, message.data, 2);
          storage.airspeed = two_byte_value;

          four_byte_value = 0;
          memcpy(conv_buf, message.data + 2, 4);
          storage.altitude = four_byte_value;

          four_byte_value = 0;
          memcpy(conv_buf, message.data + 6, 2);
          storage.vsi = (two_byte_value / 10.0);
          break;
        case LEVEL_ID:
          four_byte_value = 0;
          memcpy(conv_buf, message.data, 2);
          storage.heading = two_byte_value;

          four_byte_value = 0;
          memcpy(conv_buf, message.data + 2, 2);
          storage.roll = (two_byte_value / 10.0);

          four_byte_value = 0;
          memcpy(conv_buf, message.data + 4, 2);
          storage.pitch = (two_byte_value / 10.0);
          break;
        case GPS_ID:
          four_byte_value = 0;
          memcpy(conv_buf, message.data, 2);
          storage.ground_speed = two_byte_value;
          break;
      }
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

// void can_control_task(void* pvParameters) {
//     xSemaphoreTake(ctrl_sem, portMAX_DELAY);
//     for (int iter = 0; iter < NO_OF_ITERS; iter++) {
//         //Start CAN Driver for this iteration
//         ESP_ERROR_CHECK(can_start());
//         ESP_LOGI(TAG, "Driver started");

//         //Trigger TX and RX tasks to start transmitting/receiving
//         xSemaphoreGive(rx_sem);
//         xSemaphoreGive(tx_sem);
//         xSemaphoreTake(ctrl_sem, portMAX_DELAY);    //Wait for TX and RX tasks to finish iteration

//         ESP_ERROR_CHECK(can_stop());                //Stop the CAN Driver
//         ESP_LOGI(TAG, "Driver stopped");
//         vTaskDelay(pdMS_TO_TICKS(100));             //Delay then start next iteration
//     }
//     xSemaphoreGive(done_sem);
//     vTaskDelete(NULL);
// }
