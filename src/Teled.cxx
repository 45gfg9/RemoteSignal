#include <Teled.hxx>

int main() {
  power_all_disable();

  timer2::init();

  io::init();

  spi::init();
  wdt::init();

  rf24::init();

  set_sleep_mode(SLEEP_MODE_PWR_SAVE);
  sleep_enable();
  sei();

  timer2::await();

  for (;;) {
    sleep_cpu();
  }

  return 0;
}
