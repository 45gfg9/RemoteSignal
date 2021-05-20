#include <Teled.hxx>

int main() {
  timer2::init();

  init_io();

  spi::init();
  wdt::interrupt(wdt::wdto_1024ms);

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sei();

  timer2::await();

  for (;;) {
    loop();
  }

  return 0;
}

void init_io() {
  // PD0: LED
  // PD3: Button interrupt (PCINT19)
  DDRD = _BV(PD0) | _BV(PD3);

  set_bit(PORTD, PD3); // Pull-up
  set_bit(PCMSK2, PCINT19);
  set_bit(PCICR, PCIE2);
}

void loop() {
  sleep_cpu();
}
