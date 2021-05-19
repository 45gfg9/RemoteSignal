#include <RemoteSignal.hxx>

void wdt::interrupt(wdto_t timeout) {
  const uint8_t sreg = SREG;
  cli();

  feed();
  WDTCSR = _BV(WDCE) | _BV(WDE);
  WDTCSR = _BV(WDIE) | timeout;

  SREG = sreg;
}

void wdt::disable() {
  const uint8_t sreg = SREG;
  cli();

  feed();
  clear_bit(MCUSR, WDRF);
  WDTCSR = _BV(WDCE) | _BV(WDE);
  WDTCSR = 0;

  SREG = sreg;
}

inline void wdt::feed() {
  asm volatile("wdr");
}
