#include <teled.h>

static void rf24_tx_loop(void) {
  t2_disable_compare_b();
  t2_release();

  rf24_begin();
  t2_await();

  uint8_t payload = TCNT2 - 1 - OCR2B;
  t1_begin();

  while (!rf24_tx(payload) && !t1_timeout())
    ;

  t1_end();
  rf24_end();

  io_release();
}

ISR(PCINT2_vect) {
  _delay_ms(10); // brutally debounce

  if (io_valid()) {
    // released
    rf24_tx_loop();
  } else if (bit_is_clear(PIND, PD3)) {
    // pressed
    t2_acquire();
    t2_sync(true);
    t2_set_compare_b(TCNT2);
    io_hold();
  }

  set_bit(PCIFR, PCIF2);
}

ISR(WDT_vect) {
  if (t2_compare_a_enabled())
    return;

  // fail-safe, not tested
  wdt_set(wdt_reset, wdto_1024ms);

  rf24_begin();

  uint8_t payload;
  if (rf24_rx(&payload)) {
    led_on();
    t2_acquire();
    t2_sync(true);
    t2_set_compare_a(TCNT2 + payload);
  }
  rf24_end();
  t2_await();

  wdt_set(wdt_interrupt, wdto_1024ms);
}

ISR(TIMER2_COMPA_vect) {
  t2_disable_compare_a();
  t2_release();
  led_off();
  t2_await();
}

ISR(TIMER2_COMPB_vect) {
  rf24_tx_loop();
}
