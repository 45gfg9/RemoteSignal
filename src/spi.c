#include <teled.h>

#define SS PB2
#define MOSI PB3
#define SCK PB5

void spi_init(void) {
  set_bit(SPSR, SPI2X); // SPI freq = F_CPU/2
  set_bit(SPCR, MSTR);  // SPI Master mode
}

void spi_begin(void) {
  power_spi_enable();

  set_bit(PORTB, SS); // input pull-up
  set_bit(DDRB, MOSI);
  set_bit(DDRB, SCK);

  set_bit(SPCR, SPE); // SPI Enable
}

void spi_end(void) {
  clear_bit(SPCR, SPE);

  // reset SPI pins
  clear_bit(PORTB, SS);
  clear_bit(DDRB, MOSI);
  clear_bit(DDRB, SCK);

  power_spi_disable();
}

uint8_t spi_transfer(uint8_t data) {
  SPDR = data;
  loop_until_bit_is_set(SPSR, SPIF);
  return SPDR; // SPIF is auto cleared after this
}
