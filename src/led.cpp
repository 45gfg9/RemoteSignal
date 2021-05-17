#include <RemoteSignal.hxx>

volatile uint8_t &LED_PORT = PORTD;
const uint8_t LED_PORTBIT = PORT0;

void led_on() {
  set_bit(LED_PORT, LED_PORTBIT);
}

void led_off() {
  clear_bit(LED_PORT, LED_PORTBIT);
}
