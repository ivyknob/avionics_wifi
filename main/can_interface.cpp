#include "can_interface.hpp"

#define NO_OF_MSGS              10
#define NO_OF_ITERS             3
#define TX_GPIO_NUM             21
#define RX_GPIO_NUM             22
#define TX_TASK_PRIO            8       //Sending task priority
#define RX_TASK_PRIO            9       //Receiving task priority
#define CTRL_TSK_PRIO           10      //Control task priority
#define MSG_ID                  0x555   //11 bit standard format ID
static const char* TAG = "CAN Interface";

SemaphoreHandle_t tx_sem;
SemaphoreHandle_t rx_sem;
SemaphoreHandle_t ctrl_sem;
SemaphoreHandle_t done_sem;

void can_init() {
  tx_sem = xSemaphoreCreateBinary();
  rx_sem = xSemaphoreCreateBinary();
  ctrl_sem = xSemaphoreCreateBinary();
  done_sem = xSemaphoreCreateBinary();

  // can_general_config_t g_config = CAN_GENERAL_CONFIG_DEFAULT(GPIO_NUM_21, GPIO_NUM_22, CAN_MODE_NORMAL);
  // can_timing_config_t t_config = CAN_TIMING_CONFIG_500KBITS();
  // can_filter_config_t f_config = CAN_FILTER_CONFIG_ACCEPT_ALL();

  can_timing_config_t t_config = CAN_TIMING_CONFIG_25KBITS();
  can_filter_config_t f_config = CAN_FILTER_CONFIG_ACCEPT_ALL();
  // can_filter_config_t f_config = {.acceptance_code = (MSG_ID << 21),
  //                                 .acceptance_mask = ~(CAN_STD_ID_MASK << 21),
  //                                 .single_filter = (bool)true};
  can_general_config_t g_config = CAN_GENERAL_CONFIG_DEFAULT(GPIO_NUM_21, GPIO_NUM_22, CAN_MODE_NO_ACK);


  // if (can_driver_install(&g_config, &t_config, &f_config) == ESP_OK) {
  //   ESP_LOGI(TAG, "CAN Driver installed");
  // } else {
  //   ESP_LOGE(TAG, "Failed to install CAN Driver");
  //   return;
  // }

  // if (can_start() == ESP_OK) {
  //   ESP_LOGI(TAG, "CAN Driver started");
  // } else {
  //   ESP_LOGE(TAG, "Unable to start CAN Driver");
  //   return;
  // }

  xTaskCreatePinnedToCore(can_control_task, "CAN_ctrl", 4096, NULL, CTRL_TSK_PRIO, NULL, tskNO_AFFINITY);
  xTaskCreatePinnedToCore(can_receive_task, "CAN_rx", 4096, NULL, RX_TASK_PRIO, NULL, tskNO_AFFINITY);
  xTaskCreatePinnedToCore(can_transmit_task, "CAN_tx", 4096, NULL, TX_TASK_PRIO, NULL, tskNO_AFFINITY);

  //Install CAN driver
  ESP_ERROR_CHECK(can_driver_install(&g_config, &t_config, &f_config));
  ESP_LOGI(TAG, "Driver installed");

  //Start control task
  xSemaphoreGive(ctrl_sem);
  //Wait for all iterations and tasks to complete running
  xSemaphoreTake(done_sem, portMAX_DELAY);

  //Uninstall CAN driver
  ESP_ERROR_CHECK(can_driver_uninstall());
  ESP_LOGI(TAG, "Driver uninstalled");

  //Cleanup
  vSemaphoreDelete(tx_sem);
  vSemaphoreDelete(rx_sem);
  vSemaphoreDelete(ctrl_sem);
  vQueueDelete(done_sem);
}

void can_transmit_task(void* pvParameters) {
    can_message_t tx_msg = {.flags = CAN_MSG_FLAG_SELF, .identifier = MSG_ID, .data_length_code = 1};
    for (int iter = 0; iter < NO_OF_ITERS; iter++) {
        xSemaphoreTake(tx_sem, portMAX_DELAY);
        for (int i = 0; i < NO_OF_MSGS; i++) {
            //Transmit messages using self reception request
            tx_msg.data[0] = i;
            ESP_ERROR_CHECK(can_transmit(&tx_msg, portMAX_DELAY));
            vTaskDelay(pdMS_TO_TICKS(10));
        }
    }
    vTaskDelete(NULL);
}

void can_receive_task(void* pvParameters) {
    can_message_t rx_message;
    for (int iter = 0; iter < NO_OF_ITERS; iter++) {
        xSemaphoreTake(rx_sem, portMAX_DELAY);
        for (int i = 0; i < NO_OF_MSGS; i++) {
            //Receive message and print message data
            ESP_ERROR_CHECK(can_receive(&rx_message, portMAX_DELAY));
            ESP_LOGI(TAG, "Msg received - Data = %d", rx_message.data[0]);
        }
        //Indicate to control task all messages received for this iteration
        xSemaphoreGive(ctrl_sem);
    }
    vTaskDelete(NULL);
}

void can_control_task(void* pvParameters) {
    xSemaphoreTake(ctrl_sem, portMAX_DELAY);
    for (int iter = 0; iter < NO_OF_ITERS; iter++) {
        //Start CAN Driver for this iteration
        ESP_ERROR_CHECK(can_start());
        ESP_LOGI(TAG, "Driver started");

        //Trigger TX and RX tasks to start transmitting/receiving
        xSemaphoreGive(rx_sem);
        xSemaphoreGive(tx_sem);
        xSemaphoreTake(ctrl_sem, portMAX_DELAY);    //Wait for TX and RX tasks to finish iteration

        ESP_ERROR_CHECK(can_stop());                //Stop the CAN Driver
        ESP_LOGI(TAG, "Driver stopped");
        vTaskDelay(pdMS_TO_TICKS(100));             //Delay then start next iteration
    }
    xSemaphoreGive(done_sem);
    vTaskDelete(NULL);
}
