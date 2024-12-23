#include "emc2302.h"
#include <esp_log.h>
#include <stdint.h>
#include <freertos/FreeRTOS.h>

static const char* TAG = "emc";

#define EMC2302_1_ADDRESS 0x2e // emc2302-1
#define EMC2302_2_ADDRESS 0x2f // emc2302-2

// probe for an emc2302 at the specified address
static int
emc2302_mod_detect(i2c_master_bus_handle_t i2c, uint8_t addr){
  esp_err_t e = i2c_master_probe(i2c, addr, TIMEOUT_MS);
  if(e != ESP_OK){
    ESP_LOGI(TAG, "didn't detect EMC2302 at 0x%02x", addr);
    return -1;
  }
  return 0;
}

static int16_t
emc2302_detect_addr(i2c_master_bus_handle_t i2c, emc2302_model model){
  switch(model){
    case EMC2302_MODEL_UNSPEC:
      if(emc2302_mod_detect(i2c, EMC2302_1_ADDRESS) == 0){
        return EMC2302_1_ADDRESS;
      }
      if(emc2302_mod_detect(i2c, EMC2302_2_ADDRESS) == 0){
        return EMC2302_2_ADDRESS;
      }
      break;
    case EMC2302_MODEL_1:
      if(emc2302_mod_detect(i2c, EMC2302_1_ADDRESS) == 0){
        return EMC2302_1_ADDRESS;
      }
      break;
    case EMC2302_MODEL_2:
      if(emc2302_mod_detect(i2c, EMC2302_2_ADDRESS) == 0){
        return EMC2302_2_ADDRESS;
      }
      break;
  }
  return -1;
}

int emc2302_detect(i2c_master_bus_handle_t i2c, i2c_master_dev_handle_t* i2cemc,
                   emc2302_model model){
  int16_t addr = emc2302_detect_addr(i2c, model);
  if(addr < 0){
    ESP_LOGE(TAG, "error detecting EMC2302");
    return -1;
  }
  i2c_device_config_t devcfg = {
    .dev_addr_length = I2C_ADDR_BIT_LEN_7,
    .device_address = addr,
    .scl_speed_hz = 100000,
	};
  ESP_LOGI(TAG, "successfully detected EMC2302 at 0x%02x", addr);
  if((e = i2c_master_bus_add_device(i2c, &devcfg, i2cemc)) != ESP_OK){
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
