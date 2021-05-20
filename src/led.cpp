#include <Teled.hxx>

void led::on() {
  set_bit(PORTD, PORT0);
}

void led::off() {
  clear_bit(PORTD, PORT0);
}
