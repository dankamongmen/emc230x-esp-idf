#ifndef DANKAMONGMEN_EMC230X
#define DANKAMONGMEN_EMC230X

// ESP-IDF component for working with Microchip EMC230x 4-pin fan controllers.

#include <driver/i2c_master.h>

// in addition to the EMC2301, EMC2303, and EMC2305, there are two models of
// the EMC2302, differing in SMBus address.
typedef enum {
  EMC2301,
  EMC2302_MODEL_UNSPEC,
  EMC2302_MODEL_1,
  EMC2302_MODEL_2,
  EMC2303,
  EMC2305
} emc230x_model;

// probe the I2C bus for the specified EMC230x. if it is found, configure
// i2cemc as a device handle for it, and return 0. return non-zero on error.
int emc230x_detect(i2c_master_bus_handle_t i2c, i2c_master_dev_handle_t* i2cemc,
                   emc230x_model model);

// use the CLK pin as an push-pull output, allowing multiple devices to sync.
// this forces use of our internal oscillator as our clock source.
int emc230x_set_clockoutput(i2c_master_dev_handle_t i2cemc);

// use the CLK pin as an input.
int emc230x_set_clockinput(i2c_master_dev_handle_t i2cemc);

// use the internal oscillator as our clock, and don't replicate it on the
// CLK pin. this is the default setting.
int emc230x_set_clocklocal(i2c_master_dev_handle_t i2cemc);

// set the PWM output [0..255] for the specified fan.
int emc230x_setpwm(i2c_master_dev_handle_t i2cemc, unsigned fanidx, uint8_t pwm);

// read the tachometer for the specified fan.
int emc230x_gettach(i2c_master_dev_handle_t i2cemc, unsigned fanidx, unsigned* tach);

#endif
