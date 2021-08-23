#include <Teled.hxx>

static auto &ctx = GPIOR0;

void timer1::init() {
  OCR1A = 0xF4E; // ~ 1003ms
}

void timer1::begin() {
  power_timer1_enable();
  TCNT1 = 0;
  set_bit(TIFR1, OCF1A);
  TCCR1B = clk_div_256;
}

void timer1::end() {
  TCCR1B = stopped;
  power_timer1_disable();
}

bool timer1::timeout() {
  return bit_is_set(TIFR1, OCF1A);
}

void timer2::init() {
  power_timer2_enable();

  set_bit(ASSR, AS2);

  TCNT2 = 0;
  OCR2A = 0;
  OCR2B = 0;
  TCCR2A = 0;
  TCCR2B = 0;

  ctx = 0;
}

void timer2::set_speed(t2_speed_t speed) {
  TCCR2B = speed;
}

void timer2::acquire() {
  if (!ctx++) {
    set_speed(clk_div_128);
  }
}

void timer2::release() {
  if (!--ctx) {
    set_speed(stopped);
  }
}

void timer2::await() {
  while (ASSR & 0b11111U)
    ;
}

void timer2::sync(bool wait) {
  // Sorry TCCR2A, but you are unused now
  TCCR2A = 0;
  if (wait)
    await();
}

void timer2::advance() {
  if (TCCR2B & 0b111) {
    // Only if Timer is not stopped
    uint8_t t = TCNT2;
    while (TCNT2 == t)
      ;
  }
}

void timer2::enable_compare_a() {
  set_bit(TIMSK2, OCIE2A);
}

void timer2::enable_compare_a(uint8_t target) {
  enable_compare_a();
  OCR2A = target;

  advance();
  set_bit(TIFR2, OCF2A);
}

void timer2::disable_compare_a() {
  clear_bit(TIMSK2, OCIE2A);
}

bool timer2::compare_a_enabled() {
  return bit_is_set(TIMSK2, OCIE2A);
}

void timer2::enable_compare_b() {
  set_bit(TIMSK2, OCIE2B);
}

void timer2::enable_compare_b(uint8_t target) {
  enable_compare_b();
  OCR2B = target;

  advance();
  set_bit(TIFR2, OCF2B);
}

void timer2::disable_compare_b() {
  clear_bit(TIMSK2, OCIE2B);
}

bool timer2::compare_b_enabled() {
  return bit_is_set(TIMSK2, OCIE2B);
}
