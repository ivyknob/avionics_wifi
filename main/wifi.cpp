#include "wifi.hpp"

const int WIFI_CONNECTED_BIT = BIT0;
static int s_retry_num = 0;

esp_err_t event_handler(void *ctx, system_event_t *event) {
    const char* TAG = "wifi_setup";
    switch (event->event_id) {
    case SYSTEM_EVENT_STA_START:
        esp_wifi_connect();
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        ESP_LOGI(TAG, "got ip:%s",
                 ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
        s_retry_num = 0;
        // xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        {
            if (s_retry_num < 5) {
                esp_wifi_connect();
                // xEventGroupClearBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
                s_retry_num++;
                ESP_LOGI(TAG, "retry to connect to the AP");
            }
            ESP_LOGI(TAG, "connect to the AP fail\n");
            break;
        }
    default:
        break;
    }
    return ESP_OK;
}

void wifi_init_sta() {
    const char* TAG = "wifi_setup";
    // s_wifi_event_group = xEventGroupCreate();

    ESP_LOGI(TAG, "starting tcpip adapter");
    tcpip_adapter_init();
    nvs_flash_init();
    ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL) );

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    wifi_config_t wifi_config = {
        .sta = {
            { .ssid = "redde_2g" },
            { .password = "123123123456456456" }
        }
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
    ESP_ERROR_CHECK(esp_wifi_start() );

    ESP_LOGI(TAG, "wifi_init_sta finished.");
    ESP_LOGI(TAG, "connect to ap SSID:%s password:%s",
             "redde_2g", "123123123456456456");
}

// static EventGroupHandle_t s_wifi_event_group;

// handles WiFi events
// static esp_err_t event_handler(void* ctx, system_event_t* event) {
//   const char* TAG = "event_handler";
//   switch(event->event_id) {
//     case SYSTEM_EVENT_AP_START:
//       //ESP_ERROR_CHECK(tcpip_adapter_set_hostname(TCPIP_ADAPTER_IF_STA, "esp32"));
//       ESP_LOGI(TAG,"Access Point Started");
//       break;
//     case SYSTEM_EVENT_AP_STOP:
//       ESP_LOGI(TAG,"Access Point Stopped");
//       break;
//     case SYSTEM_EVENT_AP_STACONNECTED:
//       ESP_LOGI(TAG,"STA Connected, MAC=%02x:%02x:%02x:%02x:%02x:%02x AID=%i",
//         event->event_info.sta_connected.mac[0],event->event_info.sta_connected.mac[1],
//         event->event_info.sta_connected.mac[2],event->event_info.sta_connected.mac[3],
//         event->event_info.sta_connected.mac[4],event->event_info.sta_connected.mac[5],
//         event->event_info.sta_connected.aid);
//       break;
//     case SYSTEM_EVENT_AP_STADISCONNECTED:
//       ESP_LOGI(TAG,"STA Disconnected, MAC=%02x:%02x:%02x:%02x:%02x:%02x AID=%i",
//         event->event_info.sta_disconnected.mac[0],event->event_info.sta_disconnected.mac[1],
//         event->event_info.sta_disconnected.mac[2],event->event_info.sta_disconnected.mac[3],
//         event->event_info.sta_disconnected.mac[4],event->event_info.sta_disconnected.mac[5],
//         event->event_info.sta_disconnected.aid);
//       break;
//     case SYSTEM_EVENT_AP_PROBEREQRECVED:
//       ESP_LOGI(TAG,"AP Probe Received");
//       break;
//     case SYSTEM_EVENT_AP_STA_GOT_IP6:
//       ESP_LOGI(TAG,"Got IP6=%01x:%01x:%01x:%01x",
//         event->event_info.got_ip6.ip6_info.ip.addr[0],event->event_info.got_ip6.ip6_info.ip.addr[1],
//         event->event_info.got_ip6.ip6_info.ip.addr[2],event->event_info.got_ip6.ip6_info.ip.addr[3]);
//       break;
//     default:
//       ESP_LOGI(TAG,"Unregistered event=%i",event->event_id);
//       break;
//   }
//   return ESP_OK;
// }

////  sets up WiFi
// static void wifi_setup() {
//   const char* TAG = "wifi_setup";

//   ESP_LOGI(TAG,"starting tcpip adapter");
//   tcpip_adapter_init();
//   nvs_flash_init();
//   ESP_ERROR_CHECK(tcpip_adapter_dhcps_stop(TCPIP_ADAPTER_IF_AP));
//   //tcpip_adapter_set_hostname(TCPIP_ADAPTER_IF_AP,"esp32");
//   tcpip_adapter_ip_info_t info;
//   memset(&info, 0, sizeof(info));
//   IP4_ADDR(&info.ip, 192, 168, 4, 1);
//   IP4_ADDR(&info.gw, 192, 168, 4, 1);
//   IP4_ADDR(&info.netmask, 255, 255, 255, 0);
//   ESP_LOGI(TAG,"setting gateway IP");
//   ESP_ERROR_CHECK(tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_AP, &info));
//   //ESP_ERROR_CHECK(tcpip_adapter_set_hostname(TCPIP_ADAPTER_IF_AP,"esp32"));
//   //ESP_LOGI(TAG,"set hostname to \"%s\"",hostname);
//   ESP_LOGI(TAG,"starting DHCPS adapter");
//   ESP_ERROR_CHECK(tcpip_adapter_dhcps_start(TCPIP_ADAPTER_IF_AP));
//   //ESP_ERROR_CHECK(tcpip_adapter_set_hostname(TCPIP_ADAPTER_IF_AP,hostname));
//   ESP_LOGI(TAG,"starting event loop");
//   ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));

//   ESP_LOGI(TAG,"initializing WiFi");
//   wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
//   ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_config));
//   ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
//   ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));

//   wifi_config_t wifi_config = {
//     .ap = {
//       .ssid = AP_SSID,
//       .password= AP_PSSWD,
//       .channel = 0,
//       .authmode = WIFI_AUTH_WPA2_PSK,
//       .ssid_hidden = 0,
//       .max_connection = 4,
//       .beacon_interval = 100
//     }
//   };

//   ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
//   ESP_ERROR_CHECK(esp_wifi_start());
//   ESP_LOGI(TAG,"WiFi set up");
// }
