# Avionics WiFi

[![Maintainability](https://api.codeclimate.com/v1/badges/83ef02e6158fb5fd8ecd/maintainability)](https://codeclimate.com/github/ivyknob/avionics_wifi/maintainability)

## Requirements

### ESP-IDF

```bash
cd ~/esp
git clone -b v4.1rc --recursive https://github.com/espressif/esp-idf.git esp-idf-v4.1
cd esp-idf-v4.1
./install.sh
```

Add `. $HOME/esp/esp-idf-v4.1/export.sh > /dev/null` to `~/.zshrc` or `~/.profile`.

**Restart terminal application**

#### Python

##### installing pip

```bash
curl https://bootstrap.pypa.io/get-pip.py -o get-pip.py
python get-pip.py
rm get-pip.py
```

**After installing pip**

```bash
python -m pip install --user -r $IDF_PATH/requirements.txt
```

### Components

Inside repo run: `git submodule update --init --recursive`

### Configuration

Check esp32 port with command:

```bash
ls /dev | grep -i usb
```

copy port name.

Add something like this to your `~/.profile` or `~/.zshrc file`

```bash
export CONFIG_ESPTOOLPY_PORT="/dev/tty.usbserial-1420"
```

Reload terminal or use source command.

### sdkconfig

Copy sample file:

```bash
cp sdkconfig.sample sdkconfig
```

Run:

```bash
make menuconfig
```

* In `WiFi Configuration` select your access point.
* In `Serial Flasher Config` -> `Default serial port` enter port from above.
* `Save` -> `Exit`

## Build and Flash

Build the project and flash it to the board, then run monitor tool to view serial output:

```bash
idf.py flash monitor
```

(To exit the serial monitor, type ``Ctrl+]``.)
