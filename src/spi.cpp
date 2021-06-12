#include <Teled.hxx>

static const auto SS = PB2;
static const auto MOSI = PB3;
static const auto SCK = PB5;

void spi::init() {
  set_bit(SPSR, SPI2X); // SPI freq = F_CPU/2
}

void spi::begin() {
  power_spi_enable();

  set_bit(PORTB, SS);
  set_bit(DDRB, MOSI);
  set_bit(DDRB, SCK);

  set_bit(SPCR, MSTR); // SPI Master mode
  set_bit(SPCR, SPE);  // SPI Enable
}

void spi::end() {
  clear_bit(SPCR, SPE);

  // reset SPI pins
  clear_bit(PORTB, SS);
  clear_bit(DDRB, MOSI);
  clear_bit(DDRB, SCK);

  // clear SPI interrupt flag
  set_bit(SPSR, SPIF);

  power_spi_disable();
}

uint8_t spi::transfer(uint8_t data) {
  SPDR = data;
  loop_until_bit_is_set(SPSR, SPIF);
  return SPDR; // SPIF is auto cleared after this
}
