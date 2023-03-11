#include <teled.h>

int main() {
  t1_init();

  power_all_disable();

  t2_init();

  io_init();

  led_on();

  spi_init();
  wdt_set(wdt_interrupt, wdto_1024ms);

  rf24_init();

  set_sleep_mode(SLEEP_MODE_PWR_SAVE);
  sleep_enable();
  sei();

  led_off();

  t2_await();

  for (;;) {
    sleep_cpu();
  }

  return 0;
}
