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

// consider this struct to be opaque. it ought not be written nor read
// by application code.
typedef struct emc230x {
  int productid;
  i2c_master_dev_handle_t i2c;
  uint8_t address;
} emc230x;

// probe the I2C bus for the specified EMC230x. if it is found, configure
// i2cemc as a device handle for it, and return 0. return non-zero on error.
// this always uses the default address for the specified device; to use
// a non-standard address, see emc230x_detect_at_address() below. on success,
// the emc230x struct will be initialized for library use.
int emc230x_detect(i2c_master_bus_handle_t i2c, emc230x_model model, emc230x* emc);

// the EMC2303 and EMC2305 can use different addresses based on the address
// select pin. this function supports a non-default address (specify zero
// to use the default address for the device). an invalid address+model
// combination will result in an error being returned.
int emc230x_detect_at_address(i2c_master_bus_handle_t i2c,
                              emc230x_model model, uint8_t address,
                              emc230x* emc);

// destroy any resources held by emc, including the i2c handle.
void emc230x_destroy(emc230x* emc);

// FIXME can we make these emcs const?

// use the CLK pin as an push-pull output, allowing multiple devices to sync.
// this forces use of our internal oscillator as our clock source.
int emc230x_set_clockoutput(emc230x* emc);

// use the CLK pin as an input.
int emc230x_set_clockinput(emc230x* emc);

// use the internal oscillator as our clock, and don't replicate it on the
// CLK pin. this is the default setting.
int emc230x_set_clocklocal(emc230x* emc);

// set the PWM output [0..255] for the specified fan.
int emc230x_setpwm(emc230x* emc, unsigned fanidx, uint8_t pwm);

// read the tachometer for the specified fan. returns the direct result read
// from the register (number of 32.768 kHz cycles between measurements).
int emc230x_gettach(emc230x* emc, unsigned fanidx, unsigned* tach);

// read the tachometer for the specified fan, and convert it to rpm. assumes a
// two-pole fan reading five edges.
int emc230x_gettach_rpm(emc230x* emc, unsigned fanidx, unsigned* rpm);

#endif
