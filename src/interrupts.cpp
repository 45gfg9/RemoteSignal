#include <RemoteSignal.hxx>

ISR(WDT_vect) {
  if (rf24::available()) {
    uint8_t payload = rf24::rx();

    led::on();
    timer2::acquire();
    timer2::enable_compare_a(TCNT2 + payload);
  }
}

ISR(TIMER2_COMPA_vect) {
  timer2::disable_compare_a();
  timer2::release();
  led::off();
}

static void rf24_tx_loop() {
  cli();

  uint8_t payload = TCNT2 - OCR2B - 1; // -1 handle timeout
  OCR2B = TCNT2;

  while (!rf24::tx(payload) || bit_is_clear(TIFR2, OCF2B))
    ;

  timer2::disable_compare_b();
  timer2::release();

  sei();
}

ISR(INT1_vect) {
  // TODO debounce

  if (bit_is_clear(PIND, PD3)) {
    // pressed
    timer2::acquire();
    timer2::enable_compare_b(TCNT2);
  } else {
    // released
    rf24_tx_loop();
  }
}

ISR(TIMER2_COMPB_vect) {
  rf24_tx_loop();
}
