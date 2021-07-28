#include <Teled.hxx>

void io::init() {
  set_bit(DDRD, PD0); // LED

  set_bit(PORTD, PD3); // PCINT19 pull-up
  set_bit(PCMSK2, PCINT19);
  set_bit(PCICR, PCIE2);
}

bool io::valid() {
  return bit_is_set(SREG, SREG_T);
}

void io::hold() {
  // Set T flag
  asm volatile("set");
}

void io::release() {
  // Clear T flag
  asm volatile("clt");
}
