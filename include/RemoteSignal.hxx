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
  wdto_32ms,
  wdto_64ms,
  wdto_128ms,
  wdto_256ms,
  wdto_512ms,
  wdto_1024ms,
  wdto_2048ms,
  wdto_4096ms,
  wdto_8192ms,
};

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
