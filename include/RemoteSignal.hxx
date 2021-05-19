#ifndef __REMOTESIGNAL_HXX__
#define __REMOTESIGNAL_HXX__

#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <util/delay.h>

#define set_bit(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#define clear_bit(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))

void init_io();
void loop();

namespace wdt {
  enum wdto_t {
    wdto_16ms = 0,
    wdto_32ms = _BV(WDP0),
    wdto_64ms = _BV(WDP1),
    wdto_128ms = _BV(WDP1) | _BV(WDP0),
    wdto_256ms = _BV(WDP2),
    wdto_512ms = _BV(WDP2) | _BV(WDP0),
    wdto_1024ms = _BV(WDP2) | _BV(WDP1),
    wdto_2048ms = _BV(WDP2) | _BV(WDP1) | _BV(WDP0),
    wdto_4096ms = _BV(WDP3),
    wdto_8192ms = _BV(WDP3) | _BV(WDP0),
  };

  void interrupt(wdto_t);
  void disable();

  void feed();
} // namespace wdt

namespace timer2 {
  enum t2_speed_t {
    stopped = 0,
    clk_div_1 = _BV(CS20),
    clk_div_8 = _BV(CS21),
    clk_div_32 = _BV(CS21) | _BV(CS20),
    clk_div_64 = _BV(CS22),
    clk_div_128 = _BV(CS22) | _BV(CS20),
    clk_div_256 = _BV(CS22) | _BV(CS21),
    clk_div_1024 = _BV(CS22) | _BV(CS21) | _BV(CS20),
  };

  void init();

  void begin(t2_speed_t = clk_div_128);
  void stop();

  void acquire();
  void release();
} // namespace timer2

namespace led {
  void on();
  void off();
} // namespace led

namespace spi {
  void begin();
  void end();

  uint8_t rx();
  void tx();
} // namespace spi

namespace rf24 {
  enum rf24_mode_t { RX, TX };

  bool ready();
  bool available();

  void mode(rf24_mode_t);
  uint8_t rx();
  void tx(uint8_t);
} // namespace rf24

#endif
