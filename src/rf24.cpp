#include <RemoteSignal.hxx>
#include <RF24.h>

void rf24::begin() {
  DDRC = _BV(PC0) | _BV(PC1);

  spi::begin();
}

void rf24::end() {
  spi::end();

  DDRC = 0;
}

bool rf24::ready() {
  // TODO
  return false;
}

bool rf24::available() {
  // Stub
  return bit_is_clear(PIND, PD2);
}

uint8_t rf24::rx() {
  using spi::rx;

  return rx();
}

bool rf24::tx(uint8_t data) {
  using spi::tx;

  tx(data);

  return true;
}
