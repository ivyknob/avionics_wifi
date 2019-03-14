#include "can_interface.hpp"

#define NO_OF_MSGS              10
#define NO_OF_ITERS             3
#define TX_GPIO_NUM             21
#define RX_GPIO_NUM             22
#define TX_TASK_PRIO            8       //Sending task priority
#define RX_TASK_PRIO            9       //Receiving task priority
#define CTRL_TSK_PRIO           10      //Control task priority
#define MSG_ID                  0x555   //11 bit standard format ID
#define BLINK_GPIO GPIO_NUM_5


// SemaphoreHandle_t tx_sem;
SemaphoreHandle_t rx_sem;
// SemaphoreHandle_t ctrl_sem;
// SemaphoreHandle_t done_sem;

void can_init() {
  static const char* TAG = "CAN Init";
//   return;
//   tx_sem = xSemaphoreCreateBinary();
  // rx_sem = xSemaphoreCreateBinary();
//   ctrl_sem = xSemaphoreCreateBinary();
//   done_sem = xSemaphoreCreateBinary();
  gpio_pad_select_gpio(BLINK_GPIO);
  gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
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
  xTaskCreate(&can_receive_task, "CAN_rx", 4000, NULL, 6, NULL);
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
    gpio_set_level(BLINK_GPIO, 0);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    gpio_set_level(BLINK_GPIO, 1);
    ESP_LOGI(TAG, "Starting CAN_rx task");
    for (;;){
        bool failed = true;
        can_message_t message;
        esp_err_t res = can_receive(&message, pdMS_TO_TICKS(10000));
        switch(res) {
          case ESP_OK:
            // printf("Message received\n");
            gpio_set_level(BLINK_GPIO, 0);
            vTaskDelay(100 / portTICK_PERIOD_MS);
            gpio_set_level(BLINK_GPIO, 1);
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
            //Process received message
            // if (message.flags & CAN_MSG_FLAG_EXTD) {
            //     printf("Message is in Extended Format\n");
            // } else {
            //     printf("Message is in Standard Format\n");
            // }
            if (!(message.flags & CAN_MSG_FLAG_RTR)) {
              ESP_LOGI(TAG, "ID: %d, size: %d", message.identifier, message.data_length_code);
              // for (int i = 0; i < message.data_length_code; i++) {
              //     printf("Data byte %d = %d\n", i, message.data[i]);
              // }
            }

        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    // vTaskDelete(NULL);
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
