#ifndef DANKAMONGMEN_EMC2302
#define DANKAMONGMEN_EMC2302

// ESP-IDF component for working with Microchip EMC2302 4-pin fan controllers.

#include <driver/i2c_master.h>

// there are two models of the EMC2302, differing in SMBus address
typedef enum {
  EMC2302_MODEL_UNSPEC,
  EMC2302_MODEL_1,
  EMC2302_MODEL_2,
} emc2302_model;

// probe the I2C bus for an EMC2302. if it is found, configure i2cemc
// as a device handle for it, and return 0. return non-zero on error.
// specify the model to match only that one, or _UNSPEC to probe for
// all known models (this can lead to false positives).
int emc2302_detect(i2c_master_bus_handle_t i2c, i2c_master_dev_handle_t* i2cemc,
                   emc2302_model model);

// send the reset command w/ timeout. returns non-zero on error.
int emc2302_reset(i2c_master_dev_handle_t i2c);

#endif
