#include <Teled.hxx>

static auto &LED_PORT = PORTD;
static const auto LED_BIT = PORT0;

void led::on() {
  set_bit(LED_PORT, LED_BIT);
}

void led::off() {
  clear_bit(LED_PORT, LED_BIT);
}
