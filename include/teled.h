#ifndef __TELED_H__
#define __TELED_H__

#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stddef.h>

#define set_bit(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#define clear_bit(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))

/* io */
void io_init(void);

bool io_valid(void);

void io_hold(void);
void io_release(void);

/* watchdog */
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

enum wdt_mode_t {
  wdt_stopped = 0,
  wdt_interrupt = _BV(WDIE),
  wdt_reset = _BV(WDE),
  wdt_int_rst = _BV(WDIE) | _BV(WDE),
};

void wdt_set(enum wdt_mode_t, enum wdto_t);

/* timer 1 */
enum t1_speed_t {
  t1_stopped = 0,
  t1_clk_div_1 = _BV(CS10),
  t1_clk_div_8 = _BV(CS11),
  t1_clk_div_64 = _BV(CS11) | _BV(CS10),
  t1_clk_div_256 = _BV(CS12),
  t1_clk_div_1024 = _BV(CS12) | _BV(CS10),
  t1_ext_falling = _BV(CS12) | _BV(CS11),
  t1_ext_rising = _BV(CS12) | _BV(CS11) | _BV(CS10),
};

void t1_init(void);

void t1_begin(void);
void t1_end(void);

bool t1_timeout(void);

/* timer 2 */
enum t2_speed_t {
  t2_stopped = 0,
  t2_clk_div_1 = _BV(CS20),
  t2_clk_div_8 = _BV(CS21),
  t2_clk_div_32 = _BV(CS21) | _BV(CS20),
  t2_clk_div_64 = _BV(CS22),
  t2_clk_div_128 = _BV(CS22) | _BV(CS20),
  t2_clk_div_256 = _BV(CS22) | _BV(CS21),
  t2_clk_div_1024 = _BV(CS22) | _BV(CS21) | _BV(CS20),
};

void t2_init(void);

void t2_set_speed(enum t2_speed_t);

void t2_acquire(void);
void t2_release(void);

void t2_await(void);   // wait for registers to update
void t2_sync(bool);    // sync clk_IO with clk_Async
void t2_advance(void); // wait for Timer to advance by 1

void t2_enable_compare_a(void);
void t2_set_compare_a(uint8_t);
void t2_disable_compare_a(void);
bool t2_compare_a_enabled(void);

void t2_enable_compare_b(void);
void t2_set_compare_b(uint8_t);
void t2_disable_compare_b(void);
bool t2_compare_b_enabled(void);

/* led */
void led_on(void);
void led_off(void);

/* spi */
void spi_init(void);

void spi_begin(void);
void spi_end(void);

uint8_t spi_transfer(uint8_t);

/* rf24 */
void rf24_init(void);

void rf24_begin(void);
void rf24_end(void);

bool rf24_rx(uint8_t *);
bool rf24_tx(uint8_t);

#endif
