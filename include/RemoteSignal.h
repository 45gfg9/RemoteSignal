#ifndef __REMOTESIGNAL_H__
#define __REMOTESIGNAL_H__

#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/wdt.h>

void init();

void timer2_acquire();
void timer2_release();

void led_on();
void led_off();

void rf24_rx();
void rf24_tx(uint8_t val);

#endif
