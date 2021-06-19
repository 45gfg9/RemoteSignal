#include <Teled.hxx>

int main() {
  timer2::init();

  power_all_disable();

  io::init();

  spi::init();
  wdt::init();

  rf24::init();

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sei();

  timer2::await();

  for (;;) {
    loop();
  }

  return 0;
}

void loop() {
  sleep_cpu();
}
