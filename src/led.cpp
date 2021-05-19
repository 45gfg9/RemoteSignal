#include <RemoteSignal.hxx>

void led_on() {
  set_bit(LED_DDRR, LED_PIN);
}

void led_off() {
  clear_bit(LED_DDRR, LED_PIN);
}
