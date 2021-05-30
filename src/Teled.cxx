#include <Teled.hxx>

int main() {
  power_all_disable();

  timer2::init();

  io::init();

  spi::init();
  wdt::init();

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
