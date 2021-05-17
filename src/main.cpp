#include <RemoteSignal.hxx>

void pico_power();
void timer2_enable(bool);
void loop();

int main() {
  pico_power();
  timer2_enable(true);

  bit_set(DDRB, PB0);

  // timer2_enable(false);

  loop();

  return 0;
}

void pico_power() {
  power_all_disable();
  power_spi_enable();
}

void timer2_enable(bool enable) {
  char oldSREG = SREG;
  cli();

  bit_clear(TIMSK2, TOIE2);

  if (enable) {
    power_timer2_enable();
    bit_set(ASSR, AS2);
    TCCR2B |= 5; // clk / 128

    while (bit_is_set(ASSR, TCN2UB))
      ;
    bit_set(TIMSK2, TOIE2);
  } else {
    bit_clear(ASSR, AS2);
    TCNT2 = 0;

    while (bit_is_set(ASSR, TCR2BUB))
      ;
    bit_clear(TIMSK2, TOIE2);
    power_timer2_disable();
  }
  bit_set(TIFR2, TOV2);

  SREG = oldSREG;
}

void loop() {
  set_sleep_mode(SLEEP_MODE_PWR_SAVE);
  sleep_enable();
  sei();

  for (;;) {
    sleep_cpu();
  }
}

ISR(TIMER2_OVF_vect) {
  bit_set(PINB, PINB0);
}
