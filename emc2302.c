#include "emc2302.h"
#include <esp_log.h>
#include <freertos/FreeRTOS.h>

static const char* TAG = "emc";

#define EMC2302_ADDRESS 0x2A

int emc2302_detect(i2c_master_bus_handle_t i2c, i2c_master_dev_handle_t* i2cnau){
  const unsigned addr = EMC2302_ADDRESS;
  esp_err_t e = i2c_master_probe(i2c, addr, TIMEOUT_MS);
  if(e != ESP_OK){
    ESP_LOGE(TAG, "error %d detecting EMC2302 at 0x%02x", e, addr);
    return -1;
  }
  i2c_device_config_t devcfg = {
    .dev_addr_length = I2C_ADDR_BIT_LEN_7,
    .device_address = addr,
    .scl_speed_hz = 100000,
	};
  ESP_LOGI(TAG, "successfully detected EMC2302 at 0x%02x", addr);
  if((e = i2c_master_bus_add_device(i2c, &devcfg, i2cnau)) != ESP_OK){
    ESP_LOGE(TAG, "error %d adding emc2302 i2c device", e);
    return -1;
  }
  return 0;
}

// FIXME we'll probably want this to be async
static int
emc2302_xmit(i2c_master_dev_handle_t i2c, const void* buf, size_t blen){
  esp_err_t e = i2c_master_transmit(i2c, buf, blen, TIMEOUT_MS);
  if(e != ESP_OK){
    ESP_LOGE(TAG, "error %d transmitting %zuB via I2C", e, blen);
    return -1;
  }
  return 0;
}
