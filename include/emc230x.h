#ifndef DANKAMONGMEN_EMC230X
#define DANKAMONGMEN_EMC230X

// ESP-IDF component for working with Microchip EMC230x 4-pin fan controllers.

#include <driver/i2c_master.h>

// consider this struct to be opaque. it ought not be written nor read
// by application code.
typedef struct emc230x {
  int productid;
  i2c_master_dev_handle_t i2c;
  uint8_t address;
} emc230x;

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

// use the CLK pin as an push-pull output, allowing multiple devices to sync.
// this forces use of our internal oscillator as our clock source.
int emc230x_set_clockoutput(const emc230x* emc);

// use the CLK pin as an input.
int emc230x_set_clockinput(const emc230x* emc);

// use the internal oscillator as our clock, and don't replicate it on the
// CLK pin. this is the default setting.
int emc230x_set_clocklocal(const emc230x* emc);

// set the PWM output [0..255] for the specified fan.
int emc230x_setpwm(const emc230x* emc, unsigned fanidx, uint8_t pwm);

// read the tachometer for the specified fan. returns the direct result read
// from the register (const number of 32.768 kHz cycles between measurements).
int emc230x_gettach(const emc230x* emc, unsigned fanidx, unsigned* tach);

// read the tachometer for the specified fan, and convert it to rpm. assumes a
// two-pole fan reading five edges.
int emc230x_gettach_rpm(const emc230x* emc, unsigned fanidx, unsigned* rpm);

// by default, the alert pin is masked and will not be asserted. true
// sets/keeps the mask. false disables it.
int emc230x_set_alertmask(const emc230x* emc, bool masked);

// by default, the watchdog timer only operates at initial poweron. true
// enables the continuous watchdog. false disables it.
int emc230x_set_watchdog(const emc230x* emc, bool enabled);

// by default, interrupts are disabled for all fans. true enables interrupts
// for the specified fan. false disables them.
int emc230x_set_interrupt(const emc230x* emc, unsigned fanidx, bool enabled);

// by default, PWM is 100% at 0xff and 0% at 0x0. polarity can be inverted
// to interpret higher PWM as a lower duty cycle.
int emc230x_set_pwmpolarity(const emc230x* emc, unsigned fanidx, bool inverted);

// by default, PWM is open drain. true sets the specified PWM output to
// push-pull, false to open drain.
int emc230x_set_pwmoutput(const emc230x* emc, unsigned fanidx, bool pushpull);

typedef enum {
  EMC230X_BASE_FREQ_26000,  // 26.00 kHz, value 00
  EMC230X_BASE_FREQ_19530,  // 19.53 kHz, value 01
  EMC230X_BASE_FREQ_4882,   // 4.882 kHz, value 10
  EMC230X_BASE_FREQ_2441,   // 2.441 kHz, value 11
} emc230x_base_freq;

// set the base PWM frequency for the fan. default is EMC230X_BASE_FREQ_26000,
// which is what you want for PC fans.
int emc230x_set_pwmbasefreq(const emc230x* emc, unsigned fanidx, emc230x_base_freq freq);

typedef enum {
  EMC230X_FSR_FNSTL = 0x01,   // one or more fans have stalled
  EMC230X_FSR_FNSPIN = 0x02,  // one or more fans cannot spin up
  EMC230X_FSR_DVFAIL = 0x04,  // one or more fans cannot reach the requested speed
  EMC230X_FSR_WATCH = 0x80    // the watchdog timer has expired
}

// read the Fan Status register into fsr. this resets the EMC230X_FSR_WATCH
// bit (but no other bits). use EMC_230X_FSR_* to test bits.
int emc230x_read_fanstatus(const emc230x* emc, uint8_t* fsr);

// read the Fan Stall Status register into fss. if the error state is no
// longer occurring, reading the register clears the associated bit.
// if all bits are clear, the EMC230X_FSR_FNSTL bit will be cleared
// in the Fan Status register. the LSB is fan 1, up to 0x10 for the fifth fan
// on the EMC2305.
int emc230x_read_fanstallstatus(const emc230x* emc, uint8_t* fss);

// read the Fan Spin Status register into fss. if the error state is no
// longer occurring, reading the register clears the associated bit.
// if all bits are clear, the EMC230X_FSR_FNSPIN bit will be cleared
// in the Fan Status register. the LSB is fan 1, up to 0x10 for the fifth fan
// on the EMC2305.
int emc230x_read_fanspinstatus(const emc230x* emc, uint8_t* fss);

// read the Fan Drive Fail register into fdf. if the error state is no
// longer occurring, reading the register clears the associated bit.
// if all bits are clear, the EMC230X_FSR_DVFAIL bit will be cleared
// in the Fan Status register. the LSB is fan 1, up to 0x10 for the fifth fan
// on the EMC2305.
int emc230x_read_fanspinstatus(const emc230x* emc, uint8_t* fdf);

#endif
