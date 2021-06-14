#include <Teled.hxx>

void io::init() {
  set_bit(DDRD, PD0); // LED

  set_bit(PORTD, PD3); // PCINT19 pull-up
  set_bit(PCMSK2, PCINT19);
  set_bit(PCICR, PCIE2);

  if (bit_is_clear(PIND, PD3))
    io::press(); // button pressing
}

static auto &FLAG_PORT = GPIOR1;
static const auto FLAG_BIT = 0;

// TODO use SREG_T?
bool io::pressing() {
  return bit_is_set(FLAG_PORT, FLAG_BIT);
}

void io::press() {
  // can optimize
  set_bit(FLAG_PORT, FLAG_BIT);
}

void io::release() {
  clear_bit(FLAG_PORT, FLAG_BIT);
}
