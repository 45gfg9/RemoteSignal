#include <Teled.hxx>

static void rf24_tx_loop() {
  uint8_t payload = TCNT2 - OCR2A;
  OCR2A = TCNT2;

  while (!rf24::tx(payload) || bit_is_clear(TIFR2, OCF2A))
    ;

  timer2::disable_compare_a();
  timer2::release();
}

ISR(PCINT2_vect) {
  _delay_ms(10); // brutally debounce

  if (bit_is_clear(PIND, PD3)) {
    // pressed
    timer2::acquire();
    timer2::enable_compare_a(TCNT2);
  } else {
    // released
    rf24_tx_loop();
  }
}

ISR(WDT_vect) {
  if (rf24::available()) {
    uint8_t payload = rf24::rx();

    led::on();
    timer2::acquire();
    timer2::enable_compare_a(TCNT2 + payload);
  }
}

ISR(TIMER2_COMPA_vect) {
  rf24_tx_loop();
}

ISR(TIMER2_COMPB_vect) {
  timer2::disable_compare_b();
  timer2::release();
  led::off();
}
