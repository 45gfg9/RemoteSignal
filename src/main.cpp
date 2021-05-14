#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/power.h>

#define bit_set(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#define bit_clear(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))

void all_pin_pullup();
void pico_power();
void setup_timer2();
void loop();

int main() {
  all_pin_pullup();
  pico_power();
  setup_timer2();

  bit_set(DDRB, PB0);

  loop();

  return 0;
}

void all_pin_pullup() {
  DDRB = DDRC = DDRD = 0x00;
  PORTB = PORTC = PORTD = 0xFF;
}

void pico_power() {
  power_all_disable();
  power_timer2_enable();
  power_spi_enable();
}

void setup_timer2() {
  bit_set(ASSR, AS2);

  TCCR2B |= 5; // clk / 128

  while (bit_is_set(ASSR, TCN2UB) || bit_is_set(ASSR, TCR2BUB))
    ;
  bit_set(TIFR2, TOV2);
  bit_set(TIMSK2, TOIE2);
}

void loop() {
  set_sleep_mode(SLEEP_MODE_PWR_SAVE);
  sleep_enable();
  sei();

  for (;;) {
    sleep_cpu();
  }
}

ISR(TIMER2_OVF_vect) {
  bit_set(PINB, PINB0);
}
