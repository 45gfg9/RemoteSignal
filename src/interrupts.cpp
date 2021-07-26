#include <Teled.hxx>

static void rf24_tx_loop() {
  timer2::sync();

  uint8_t payload = TCNT2 - 1 - OCR2B;
  OCR2B = TCNT2;

  while (!rf24::tx(payload) || bit_is_clear(TIFR2, OCF2B))
    ;

  timer2::disable_compare_b();
  timer2::release();

  set_bit(TIFR2, OCF2B);

  io::release();
}

ISR(PCINT2_vect) {
  _delay_ms(10); // brutally debounce

  if (io::valid()) {
    // released
    rf24_tx_loop();
  } else if (bit_is_clear(PIND, PD3)) {
    // pressed
    timer2::acquire();
    timer2::sync();
    timer2::enable_compare_b(TCNT2);
    io::hold();
  }
  timer2::await();
}

ISR(WDT_vect) {
  if (rf24::available()) {
    uint8_t payload = rf24::rx();

    led::on();
    timer2::acquire();
    timer2::enable_compare_a(TCNT2 + payload);
    timer2::await();
  }
}

ISR(TIMER2_COMPA_vect) {
  timer2::disable_compare_a();
  timer2::release();
  led::off();
}

ISR(TIMER2_COMPB_vect) {
  rf24_tx_loop();
  timer2::await();
}
