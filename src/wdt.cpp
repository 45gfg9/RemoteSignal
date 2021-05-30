#include <Teled.hxx>

inline static void wdr() {
  asm volatile("wdr");
}

void wdt::init(wdto_t timeout) {
  const uint8_t sreg = SREG;
  cli();

  wdr();
  WDTCSR = _BV(WDCE) | _BV(WDE);
  WDTCSR = _BV(WDIE) | timeout;

  SREG = sreg;
}

void wdt::disable() {
  const uint8_t sreg = SREG;
  cli();

  wdr();
  clear_bit(MCUSR, WDRF);
  WDTCSR = _BV(WDCE) | _BV(WDE);
  WDTCSR = 0;
  SREG = sreg;
}
