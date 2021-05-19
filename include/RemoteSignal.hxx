#ifndef __REMOTESIGNAL_HXX__
#define __REMOTESIGNAL_HXX__

#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <util/delay.h>

#define set_bit(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#define clear_bit(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))

enum wdto_t {
  wdto_16ms = 0,
  wdto_32ms = _BV(WDP0),
  wdto_64ms = _BV(WDP1),
  wdto_128ms = _BV(WDP1) | _BV(WDP0),
  wdto_256ms = _BV(WDP2),
  wdto_512ms = _BV(WDP2) | _BV(WDP0),
  wdto_1024ms = _BV(WDP2) | _BV(WDP1),
  wdto_2048ms = _BV(WDP2) | _BV(WDP1) | _BV(WDP0),
  wdto_4096ms = _BV(WDP3),
  wdto_8192ms = _BV(WDP3) | _BV(WDP0),
};

static volatile uint8_t &LED_DDRR = DDRD;
static volatile uint8_t &LED_PORTR = PORTD;
static volatile uint8_t &LED_PINR = PIND;
static const uint8_t LED_PIN = PD0;

void init_io();
void init_spi();
void init_rf24();
void init_timer2();

void wdt_enable(wdto_t);

void timer2_acquire();
void timer2_release();

void led_on();
void led_off();

void rf24_rx();
void rf24_tx(uint8_t);

#endif
