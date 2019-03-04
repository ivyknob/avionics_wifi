# Avionics WiFi

## Requirements

* ESP-IDF (git clone -b v3.1.3 --recursive https://github.com/espressif/esp-idf.git esp-idf)
* BNO055ESP32
* Websocket server
* Init submodules `git submodule update --init --recursive`

## Build and Flash

Build the project and flash it to the board, then run monitor tool to view serial output:

```
make -j4 flash monitor
make uploadfs
```

(To exit the serial monitor, type ``Ctrl-]``.)
