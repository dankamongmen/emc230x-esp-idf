An [esp-idf](https://github.com/espressif/esp-idf) component for interfacing
with the [EMC230x](https://ww1.microchip.com/downloads/aemDocuments/documents/MSLD/ProductDocuments/DataSheets/EMC2301-2-3-5-Data-Sheet-DS20006532A.pdf)
line of 4-pin fan controllers. This includes EMC2301, EMC2302, EMC2303, and EMC2305.

This is present in the [Component Registry](https://components.espressif.com/)
as [dankamongmen/emc230x](https://components.espressif.com/components/dankamongmen/emc230x).

The EMC230x provides an SMBus 2.0 interface compatible with most modes
of I²C. It provides independent tachometry and PWM control for each
attached fan. It does not provide power for the fans. The tach signals
can be up to 5V; the device itself is powered using 3.3V. All models
consume less than a milliamp. The tachometer supports between 480 and
16000 RPM. PWM can be set directly, or managed by the device using a
tachometer target expressed in 32.768 kHz cycles. The SMBus pins are
all 5V-tolerant. An alert pin can report fan stalls. The device has an
internal oscillator, but can use an external one.

Compared to directly managing the fans, the EMC230x can reduce pin count,
eliminate the need for a tachometer interrupt, offload tach-based PWM
management, and force the tach signal to the SMBus voltage.

The EMC2301 and both models of the EMC2302 have fixed SMBus addresses,
but the EMC2302 and EMC2305 support six different ones based on the
address select pin.

[![Component Registry](https://components.espressif.com/components/dankamongmen/emc230x/badge.svg)](https://components.espressif.com/components/dankamongmen/emc230x)
