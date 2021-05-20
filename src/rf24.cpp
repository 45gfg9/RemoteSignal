#include <RemoteSignal.hxx>
#include <RF24.h>

void begin() {
  DDRC = _BV(PC0) | _BV(PC1);

  spi::begin();
}

void end() {
  spi::end();

  DDRC = 0;
}

uint8_t rx() {
  using spi::rx;

  return rx();
}

bool tx(uint8_t data) {
  using spi::tx;

  tx(data);
}
