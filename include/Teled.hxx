#ifndef __TELED_HXX__
#define __TELED_HXX__

#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#define set_bit(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#define clear_bit(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))

namespace io {
  void init();

  bool pressing();

  void press();
  void release();
} // namespace io

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

  void init(wdto_t = wdto_1024ms);
  void disable();
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

  void await();
  void sync();

  void enable_compare_a();
  void enable_compare_a(uint8_t);
  void disable_compare_a();

  void enable_compare_b();
  void enable_compare_b(uint8_t);
  void disable_compare_b();
} // namespace timer2

namespace led {
  void on();
  void off();
} // namespace led

namespace spi {
  void init();

  void begin();
  void end();

  uint8_t transfer(uint8_t);
} // namespace spi

namespace rf24 {
  void init();

  void begin();
  void end();

  bool available();

  uint8_t rx();
  bool tx(uint8_t);
} // namespace rf24

#endif
