#include <Teled.hxx>

static auto &ctx = GPIOR0;

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

void timer2::acquire() {
  if (!ctx++) {
    TCCR2B = clk_div_128;
  }
}

void timer2::release() {
  if (!--ctx) {
    TCCR2B = stopped;
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

// https://www.avrfreaks.net/forum/timer-2-async-compare-match-isr-executed-when-it-shouldnt
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
