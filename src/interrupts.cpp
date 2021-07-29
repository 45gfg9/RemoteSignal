#include <Teled.hxx>

static void rf24_tx_loop() {
  timer2::disable_compare_b();
  timer2::release();

  rf24::begin();
  timer2::await();

  uint8_t payload = TCNT2 - 1 - OCR2B;
  timer1::begin();

  while (!rf24::tx(payload) && !timer1::timeout())
    ;

  timer1::end();
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

  set_bit(PCIFR, PCIF2);
}

ISR(WDT_vect) {
  if (timer2::compare_a_enabled())
    return;

  rf24::begin();

  uint8_t payload;
  if (rf24::rx(&payload)) {
    led::on();
    timer2::acquire();
    timer2::sync();
    timer2::enable_compare_a(TCNT2 + payload);
  }
  rf24::end();
  timer2::await();
}

ISR(TIMER2_COMPA_vect) {
  timer2::disable_compare_a();
  timer2::release();
  led::off();
  timer2::await();
}

ISR(TIMER2_COMPB_vect) {
  rf24_tx_loop();
}
