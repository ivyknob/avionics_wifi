# Avionics WiFi

## Requirements

### ESP Toolchain

```
mkdir ~/esp
cd ~/esp
wget https://dl.espressif.com/dl/xtensa-esp32-elf-osx-1.22.0-80-g6c4433a-5.2.0.tar.gz
tar -xzf ~/Downloads/xtensa-esp32-elf-osx-1.22.0-80-g6c4433a-5.2.0.tar.gz
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

### Components

Inside repo run: `git submodule update --init --recursive`

## Build and Flash

Build the project and flash it to the board, then run monitor tool to view serial output:

```
make -j4 flash monitor
```

(To exit the serial monitor, type ``Ctrl-]``.)
