#include "webserver.hpp"

static QueueHandle_t client_queue;
static const int client_queue_size = 10;

// handles websocket events
void websocket_callback(uint8_t num, WEBSOCKET_TYPE_t type, char* msg, uint64_t len) {
  static const char* TAG = "websocket_callback";
  int value;

  switch (type) {
    case WEBSOCKET_CONNECT:
      ESP_LOGI(TAG, "client %i connected!", num);
      break;
    case WEBSOCKET_DISCONNECT_EXTERNAL:
      ESP_LOGI(TAG, "client %i sent a disconnect message", num);
      break;
    case WEBSOCKET_DISCONNECT_INTERNAL:
      ESP_LOGI(TAG, "client %i was disconnected", num);
      break;
    case WEBSOCKET_DISCONNECT_ERROR:
      ESP_LOGI(TAG, "client %i was disconnected due to an error", num);
      break;
    case WEBSOCKET_TEXT:
      if (len) {
        switch (msg[0]) {
          case 'L':
            if (sscanf(msg, "L%i", &value)) {
              ws_server_send_text_all_from_callback(msg, len);  // broadcast it!
            }
        }
      }
      break;
    case WEBSOCKET_BIN:
      ESP_LOGI(TAG, "client %i sent binary message of size %i:\n%s", num, (uint32_t)len, msg);
      break;
    case WEBSOCKET_PING:
      ESP_LOGI(TAG, "client %i pinged us with message of size %i:\n%s", num, (uint32_t)len, msg);
      break;
    case WEBSOCKET_PONG:
      ESP_LOGI(TAG, "client %i responded to the ping", num);
      break;
  }
}

// serves any clients
void http_serve(struct netconn *conn) {
  static const char* TAG = "http_server";
  static const char HTML_HEADER[] = "HTTP/1.1 200 OK\nContent-type: text/html\n\n";
  // static const char ERROR_HEADER[] = "HTTP/1.1 404 Not Found\nContent-type: text/html\n\n";
  static const char JS_HEADER[] = "HTTP/1.1 200 OK\nContent-type: text/javascript\n\n";
  static const char JS_GZIP_HEADER[] = "HTTP/1.1 200 OK\nContent-type: text/javascript\nContent-Encoding: gzip\n\n";
  // static const char CSS_HEADER[] = "HTTP/1.1 200 OK\nContent-type: text/css\n\n";
  //  static const char PNG_HEADER[] = "HTTP/1.1 200 OK\nContent-type: image/png\n\n";
  static const char ICO_HEADER[] = "HTTP/1.1 200 OK\nContent-type: image/x-icon\n\n";
  //  static const char PDF_HEADER[] = "HTTP/1.1 200 OK\nContent-type: application/pdf\n\n";
  //  static const char EVENT_HEADER[] = "HTTP/1.1 200 OK\nContent-Type: text/event-stream\nCache-Control: no-cache\nretry: 3000\n\n";
  struct netbuf* inbuf;
  static char* buf;
  static uint16_t buflen;
  static err_t err;

  //  default page
  extern const uint8_t index_html_start[] asm("_binary_index_html_start");
  extern const uint8_t index_html_end[] asm("_binary_index_html_end");
  const uint32_t index_html_len = index_html_end - index_html_start;

  // avionics.js
  extern const uint8_t avionics_js_start[] asm("_binary_avionics_js_start");
  extern const uint8_t avionics_js_end[] asm("_binary_avionics_js_end");
  const uint32_t avionics_js_len = avionics_js_end - avionics_js_start;

  // avionics.js.gz
  extern const uint8_t avionics_js_gz_start[] asm("_binary_avionics_js_gz_start");
  extern const uint8_t avionics_js_gz_end[] asm("_binary_avionics_js_gz_end");
  const uint32_t avionics_js_gz_len = avionics_js_gz_end - avionics_js_gz_start;

  // favicon.ico
  extern const uint8_t favicon_ico_start[] asm("_binary_favicon_ico_start");
  extern const uint8_t favicon_ico_end[] asm("_binary_favicon_ico_end");
  const uint32_t favicon_ico_len = favicon_ico_end - favicon_ico_start;

  netconn_set_recvtimeout(conn, 1000);  //  allow a connection timeout of 1 second
  ESP_LOGI(TAG, "reading from client...");
  err = netconn_recv(conn, &inbuf);
  ESP_LOGI(TAG, "read from client");
  if (err == ERR_OK) {
    netbuf_data(inbuf, (void**)&buf, &buflen);
    if (buf) {
      //  default page
      if (strstr(buf, "GET / ") && !strstr(buf, "Upgrade: websocket")) {
        ESP_LOGI(TAG, "Sending /");
        netconn_write(conn, HTML_HEADER, sizeof(HTML_HEADER) - 1, NETCONN_NOCOPY);
        netconn_write(conn, index_html_start, index_html_len, NETCONN_NOCOPY);
        netconn_close(conn);
        netconn_delete(conn);
        netbuf_delete(inbuf);
      } else if (strstr(buf, "GET / ") && strstr(buf, "Upgrade: websocket")) {
        // default page websocket
        ESP_LOGI(TAG, "Requesting websocket on /");
        ws_server_add_client(conn, buf, buflen, (char*)"/", websocket_callback);
        netbuf_delete(inbuf);
      } else if (strstr(buf, "GET /avionics.js ")) {
        ESP_LOGI(TAG, "Sending /avionics.js");
        netconn_write(conn, JS_HEADER, sizeof(JS_HEADER) - 1, NETCONN_NOCOPY);
        netconn_write(conn, avionics_js_start, avionics_js_len, NETCONN_NOCOPY);
        netconn_close(conn);
        netconn_delete(conn);
        netbuf_delete(inbuf);
      } else if (strstr(buf, "GET /avionics.js.gz ")) {
        ESP_LOGI(TAG, "Sending /avionics.js.gz");
        netconn_write(conn, JS_GZIP_HEADER, sizeof(JS_GZIP_HEADER) - 1, NETCONN_NOCOPY);
        netconn_write(conn, avionics_js_gz_start, avionics_js_gz_len, NETCONN_NOCOPY);
        netconn_close(conn);
        netconn_delete(conn);
        netbuf_delete(inbuf);
      } else if (strstr(buf, "GET /favicon.ico ")) {
        ESP_LOGI(TAG, "Sending favicon.ico");
        netconn_write(conn, ICO_HEADER, sizeof(ICO_HEADER) - 1, NETCONN_NOCOPY);
        netconn_write(conn, favicon_ico_start, favicon_ico_len, NETCONN_NOCOPY);
        netconn_close(conn);
        netconn_delete(conn);
        netbuf_delete(inbuf);
      // } else if (strstr(buf, "GET /")) {
      //   ESP_LOGI(TAG, "Unknown request, sending error page: %s", buf);
      //   netconn_write(conn, ERROR_HEADER, sizeof(ERROR_HEADER) - 1, NETCONN_NOCOPY);
      //   netconn_write(conn, error_html_start, error_html_len, NETCONN_NOCOPY);
      //   netconn_close(conn);
      //   netconn_delete(conn);
      //   netbuf_delete(inbuf);
      } else {
        ESP_LOGI(TAG, "Unknown request");
        netconn_close(conn);
        netconn_delete(conn);
        netbuf_delete(inbuf);
      }
    } else {
      ESP_LOGI(TAG, "Unknown request (empty?...)");
      netconn_close(conn);
      netconn_delete(conn);
      netbuf_delete(inbuf);
    }
  } else {  //  if err==ERR_OK
    ESP_LOGI(TAG, "error on read, closing connection");
    netconn_close(conn);
    netconn_delete(conn);
    netbuf_delete(inbuf);
  }
}

// handles clients when they first connect. passes to a queue
void server_task(void* pvParameters) {
  static const char* TAG = "server_task";
  struct netconn *conn, *newconn;
  static err_t err;
  client_queue = xQueueCreate(client_queue_size, sizeof(struct netconn*));

  conn = netconn_new(NETCONN_TCP);
  netconn_bind(conn, NULL, 80);
  netconn_listen(conn);
  ESP_LOGI(TAG, "server listening");
  do {
    err = netconn_accept(conn, &newconn);
    ESP_LOGI(TAG, "new client");
    if (err == ERR_OK) {
      xQueueSendToBack(client_queue, &newconn, portMAX_DELAY);
      // http_serve(newconn);
    }
  } while (err == ERR_OK);
  netconn_close(conn);
  netconn_delete(conn);
  ESP_LOGE(TAG, "task ending, rebooting board");
  esp_restart();
}

// receives clients from queue, handles them
void server_handle_task(void* pvParameters) {
  static const char* TAG = "server_handle_task";
  struct netconn* conn;
  ESP_LOGI(TAG, "task starting");
  for (;;) {
    xQueueReceive(client_queue, &conn, portMAX_DELAY);
    if (!conn) continue;
    http_serve(conn);
  }
  vTaskDelete(NULL);
}

void websockets_task(void* pvParameters) {
  static const char* TAG = "websockets_task";
  int n = 0;
  const int DELAY = 100 / portTICK_PERIOD_MS;

  ESP_LOGI(TAG, "starting task");
  for (;;) {

    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "pitch", (int)storage.pitch);
    cJSON_AddNumberToObject(root, "roll", (int)storage.roll);
    cJSON_AddNumberToObject(root, "heading", (int)storage.heading);

    char *rendered = cJSON_PrintUnformatted(root);
    int64_t len = strlen(rendered);
    ws_server_send_text_all(rendered, len);
    cJSON_Delete(root);

    n++;
    if (n > 10) {
      n = 0;
      ESP_LOGI(TAG, "rendered json: %s", rendered);
    }
    vTaskDelay(DELAY);
  }
}
