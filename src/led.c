#include <teled.h>

#define LED_PORT PORTD
#define LED_BIT 0

void led_on(void) {
  set_bit(LED_PORT, LED_BIT);
}

void led_off(void) {
  clear_bit(LED_PORT, LED_BIT);
}
