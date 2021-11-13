#include <Teled.hxx>

void io::init() {
  MCUSR = 0; // clear all

  set_bit(DDRD, PD0); // LED

  set_bit(PORTD, PD3); // PCINT19 pull-up
  set_bit(PCMSK2, PCINT19);
  set_bit(PCICR, PCIE2);
}

static auto &FLAG_PORT = GPIOR1;
static const auto FLAG_BIT = 0;

bool io::valid() {
  return bit_is_set(FLAG_PORT, FLAG_BIT);
}

void io::hold() {
  // can optimize
  set_bit(FLAG_PORT, FLAG_BIT);
}

void io::release() {
  clear_bit(FLAG_PORT, FLAG_BIT);
}
