#include <Teled.hxx>

static void rf24_tx_loop() {
  timer2::sync(false);
  rf24::begin();
  timer2::await();

  uint8_t payload = TCNT2 - 1 - OCR2B;
  OCR2B = TCNT2;

  while (!rf24::tx(payload) && bit_is_clear(TIFR2, OCF2B))
    ;

  timer2::disable_compare_b();
  timer2::release();

  set_bit(TIFR2, OCF2B);

  rf24::end();

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
  rf24::begin();

  uint8_t payload;
  if (rf24::rx(&payload)) {
    led::on();
    timer2::acquire();
    timer2::sync();
    // FIXME: Possible overlap causing extra acquisition
    timer2::enable_compare_a(TCNT2 + payload);
  }
  rf24::end();
  timer2::await();
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
