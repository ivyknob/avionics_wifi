# Avionics WiFi

[![Maintainability](https://api.codeclimate.com/v1/badges/83ef02e6158fb5fd8ecd/maintainability)](https://codeclimate.com/github/ivyknob/avionics_wifi/maintainability)

## Requirements

### ESP Toolchain

**MAC OS X**
```
mkdir ~/esp
cd ~/esp
wget https://dl.espressif.com/dl/xtensa-esp32-elf-osx-1.22.0-80-g6c4433a-5.2.0.tar.gz
tar -xzf ~/esp/xtensa-esp32-elf-osx-1.22.0-80-g6c4433a-5.2.0.tar.gz
```
**UBUNTU**

```
sudo apt-get install gcc git wget make libncurses-dev flex bison gperf python python-pip python-setuptools python-serial python-cryptography python-future python-pyparsing
mkdir ~/esp
cd ~/esp
wget https://dl.espressif.com/dl/xtensa-esp32-elf-linux64-1.22.0-80-g6c4433a-5.2.0.tar.gz
tar -xzf ~/esp/xtensa-esp32-elf-linux64-1.22.0-80-g6c4433a-5.2.0.tar.gz
```

Add `$HOME/esp/xtensa-esp32-elf/bin` to `~/.zshrc` or `~/.profile`.
It should look something like:
```
export PATH=$HOME/esp/xtensa-esp32-elf/bin:$PATH
```

### ESP-IDF

```
cd ~/esp
git clone -b v3.3-beta3 --recursive https://github.com/espressif/esp-idf.git
```
Add `export IDF_PATH="$HOME/esp/esp-idf"` to `~/.zshrc` or `~/.profile`.

**Restart terminal application**

#### Python

##### installing pip

```
curl https://bootstrap.pypa.io/get-pip.py -o get-pip.py
python get-pip.py
rm get-pip.py
```

**After installing pip**

```
python -m pip install --user -r $IDF_PATH/requirements.txt
```

### Components

Inside repo run: `git submodule update --init --recursive`

### Configuration

Check esp32 port with command:

```
ls /dev | grep -i usb
```

copy port name.

Add something like this to your `~/.profile` or `~/.zshrc file`

```
export CONFIG_ESPTOOLPY_PORT="/dev/tty.usbserial-1420"
```

Reload terminal or use source command.

### sdkconfig

Copy sample file:

```
cp sdkconfig.sample sdkconfig
```

Run:

```
make menuconfig
```

* In `WiFi Configuration` select your access point.
* In `Serial Flasher Config` -> `Default serial port` enter port from above.
* `Save` -> `Exit`

## Build and Flash

Build the project and flash it to the board, then run monitor tool to view serial output:

```
make -j4 flash monitor
```

(To exit the serial monitor, type ``Ctrl+]``.)
