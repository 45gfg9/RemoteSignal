#include <Teled.hxx>

int main() {
  timer1::init();

  power_all_disable();

  timer2::init();

  io::init();

  led::on();

  spi::init();
  wdt::init();

  rf24::init();

  set_sleep_mode(SLEEP_MODE_PWR_SAVE);
  sleep_enable();
  sei();

  led::off();

  timer2::await();

  for (;;) {
    sleep_cpu();
  }

  return 0;
}
