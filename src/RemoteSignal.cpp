#include <RemoteSignal.hxx>

int main() {
  init_io();

  timer2::init();
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
  // LED
  set_bit(DDRD, PD0);

  // Button interrupt (PCINT19)
  set_bit(DDRD, PD3);
  set_bit(PORTD, PD3); // Pull-up
  set_bit(PCMSK2, PCINT19);
  set_bit(PCICR, PCIE2);

  // SPI
  set_bit(DDRB, PB3); // MOSI
  set_bit(DDRB, PB5); // SCK

  // RF24
  set_bit(DDRC, PC0); // CSN
  set_bit(DDRC, PC1); // CE
}

void loop() {
  sleep_cpu();
}
