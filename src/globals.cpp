#include <RemoteSignal.hxx>

void init_io() {
  // LED
  set_bit(DDRD, PD0);

  // Button interrupt (INT1)
  set_bit(DDRD, PD3);
  set_bit(PORTD, PD3); // Pull-up

  // SPI
  set_bit(DDRB, PB3); // MOSI
  set_bit(DDRB, PB5); // SCK

  // RF24 CSN
  set_bit(DDRC, PC0);

  // RF24 CE
  set_bit(DDRC, PC1);
}
