#include <teled.h>

void wdt_set(enum wdt_mode_t mode, enum wdto_t timeout) {
  const uint8_t sreg = SREG;
  cli();

  asm volatile("wdr");

  clear_bit(MCUSR, WDRF);
  WDTCSR = _BV(WDCE) | _BV(WDE);
  WDTCSR = mode | timeout;

  SREG = sreg;
}
