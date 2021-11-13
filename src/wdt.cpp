#include <Teled.hxx>

void wdt::set(wdt_mode_t mode, wdto_t timeout) {
  const uint8_t sreg = SREG;
  cli();

  asm volatile("wdr");

  clear_bit(MCUSR, WDRF);
  WDTCSR = _BV(WDCE) | _BV(WDE);
  WDTCSR = mode | timeout;

  SREG = sreg;
}
