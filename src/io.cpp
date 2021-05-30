#include <Teled.hxx>

void io::init() {
  set_bit(DDRD, PD0); // LED

  set_bit(PORTD, PD3); // Pull-up
  set_bit(PCMSK2, PCINT19);
  set_bit(PCICR, PCIE2);

  if (bit_is_clear(PIND, PD3))
    io::press(); // button pressing
}
