#include "gyro.hpp"

BNO055* bno = NULL;
bool gyro_ok = false;

void gyro_init() {
  static const char *TAG = "BNO055";

  bno = new BNO055(UART_NUM_1, GPIO_NUM_17, GPIO_NUM_16);

  // bno = new BNO055((i2c_port_t)I2C_NUM_0, 0x29); // BNO055 I2C Addr can be 0x28 or 0x29 (depends on your hardware)

  try{
		bno->begin(); //BNO055 is in CONFIG_MODE until it is changed
		bno->enableExternalCrystal();
		//bno.setSensorOffsets(storedOffsets);
		//bno.setAxisRemap(BNO055_REMAP_CONFIG_P1, BNO055_REMAP_SIGN_P1); // see datasheet, section 3.4
		bno->setOprModeNdof();
    gyro_ok = true;
		ESP_LOGI(TAG, "Setup Done.");
	}
	catch (BNO055BaseException& ex){ //see BNO055ESP32.h for more details about exceptions
		ESP_LOGE(TAG, "Setup Failed, Error: %s", ex.what());
		return;
	}
	catch (std::exception& ex){
		ESP_LOGE(TAG, "Setup Failed, Error: %s", ex.what());
		return;
	}

	try{
		int8_t temperature = bno->getTemp();
		ESP_LOGI(TAG, "TEMP: %d°C", temperature);

		int16_t sw = bno->getSWRevision();
		uint8_t bl_rev = bno->getBootloaderRevision();
		ESP_LOGI(TAG, "SW rev: %d, bootloader rev: %u", sw, bl_rev);

		bno055_self_test_result_t res = bno->getSelfTestResult();
		ESP_LOGI(TAG, "Self-Test Results: MCU: %u, GYR:%u, MAG:%u, ACC: %u",res.mcuState,res.gyrState,res.magState,res.accState);
	}
	catch (BNO055BaseException& ex){ //see BNO055ESP32.h for more details about exceptions
		ESP_LOGE(TAG, "Something bad happened: %s", ex.what());
		return;
	}
	catch (std::exception& ex){
		ESP_LOGE(TAG, "Something bad happened: %s", ex.what());
		return;
	}
}

  