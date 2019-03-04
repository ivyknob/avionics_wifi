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

Add `$HOME/esp/xtensa-esp32-elf/bin` to ~/.zshrc or ~/.profile .
It should look something like:
```
export PATH=$HOME/esp/xtensa-esp32-elf/bin:$PATH
```

### ESP-IDF

```
cd ~/esp
git clone --recursive https://github.com/espressif/esp-idf.git
```
Add `export IDF_PATH="$HOME/esp/esp-idf"` to ~/.zshrc or ~/.profile .

**Restart terminal application**

#### Python

**Ubuntu**
```
sudo easy_install pip
```

**Mac OS X**

**After installing pip**

```
python -m pip install --user -r $IDF_PATH/requirements.txt
```

### Components

Inside repo run: `git submodule update --init --recursive`

### Configuration

Check esp32 port with command:

```
ls /dev | grep usb
```
And copy it.

**Ubuntu**

```
cp sdkconfig.ubuntu sdkconfig
```

Edit `sdkconfig` and insert correct port from above.

**MAC OS X**

```
cp sdkconfig.osx sdkconfig
```

Edit `sdkconfig` and insert correct port from above.

**Run after cp**

Run command to generate full configuration file:

```
make defconfig
```

**If there are new configuration found in sdk please commit changes back to sdkconfig.defaults**

## Build and Flash

Build the project and flash it to the board, then run monitor tool to view serial output:

```
make -j4 flash monitor
```

(To exit the serial monitor, type ``Ctrl-]``.)
