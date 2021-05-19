#include <RemoteSignal.hxx>

void led::on() {
  set_bit(PORTD, PORT0);
}

void led::off() {
  clear_bit(PORTD, PORT0);
}
