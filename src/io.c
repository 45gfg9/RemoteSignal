#include <teled.h>

#define FLAG_PORT GPIOR1
#define FLAG_BIT 0

void io_init(void) {
  MCUSR = 0; // clear all

  set_bit(DDRD, PD0); // LED

  set_bit(PORTD, PD3); // PCINT19 pull-up
  set_bit(PCMSK2, PCINT19);
  set_bit(PCICR, PCIE2);
}

bool io_valid(void) {
  return bit_is_set(FLAG_PORT, FLAG_BIT);
}

void io_hold(void) {
  // can optimize
  set_bit(FLAG_PORT, FLAG_BIT);
}

void io_release(void) {
  clear_bit(FLAG_PORT, FLAG_BIT);
}
