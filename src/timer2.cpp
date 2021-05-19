#include <RemoteSignal.hxx>

void timer2::init() {
  set_bit(ASSR, AS2);

  // stub

  loop_until_bit_is_clear(ASSR, TCR2BUB);
}

void timer2::enable_compare_a() {
  set_bit(TIMSK2, OCIE2A);
}

void timer2::enable_compare_a(uint8_t target) {
  enable_compare_a();
  OCR2A = target;
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
}

void timer2::disable_compare_b() {
  clear_bit(TIMSK2, OCIE2B);
}
