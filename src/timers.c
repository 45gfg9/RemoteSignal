#include <teled.h>

#define ctx GPIOR0

void t1_init(void) {
  OCR1A = 0xF4E; // ~ 1003ms
}

void t1_begin(void) {
  power_timer1_enable();
  TCNT1 = 0;
  set_bit(TIFR1, OCF1A);
  TCCR1B = t1_clk_div_256;
}

void t1_end(void) {
  TCCR1B = t1_stopped;
  power_timer1_disable();
}

bool t1_timeout(void) {
  return bit_is_set(TIFR1, OCF1A);
}

void t2_init(void) {
  power_timer2_enable();

  set_bit(ASSR, AS2);

  TCNT2 = 0;
  OCR2A = 0;
  OCR2B = 0;
  TCCR2A = 0;
  TCCR2B = 0;

  ctx = 0;
}

void t2_set_speed(enum t2_speed_t speed) {
  TCCR2B = speed;
}

void t2_acquire(void) {
  if (!ctx++) {
    t2_set_speed(t2_clk_div_256);
  }
}

void t2_release(void) {
  if (!--ctx) {
    t2_set_speed(t2_stopped);
  }
}

void t2_await(void) {
  while (ASSR & 0b11111U)
    ;
}

void t2_sync(bool wait) {
  // Sorry TCCR2A, but you are unused now
  TCCR2A = 0;
  if (wait)
    t2_await();
}

void t2_advance(void) {
  if (TCCR2B & 0b111) {
    // Only if Timer is not stopped
    uint8_t t = TCNT2;
    while (TCNT2 == t)
      ;
  }
}

void t2_enable_compare_a(void) {
  set_bit(TIMSK2, OCIE2A);
}

void t2_set_compare_a(uint8_t target) {
  t2_enable_compare_a();
  OCR2A = target;

  t2_advance();
  set_bit(TIFR2, OCF2A);
}

void t2_disable_compare_a(void) {
  clear_bit(TIMSK2, OCIE2A);
}

bool t2_compare_a_enabled(void) {
  return bit_is_set(TIMSK2, OCIE2A);
}

void t2_enable_compare_b(void) {
  set_bit(TIMSK2, OCIE2B);
}

void t2_set_compare_b(uint8_t target) {
  t2_enable_compare_b();
  OCR2B = target;

  t2_advance();
  set_bit(TIFR2, OCF2B);
}

void t2_disable_compare_b(void) {
  clear_bit(TIMSK2, OCIE2B);
}

bool t2_compare_b_enabled(void) {
  return bit_is_set(TIMSK2, OCIE2B);
}
