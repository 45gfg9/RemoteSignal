#include <RemoteSignal.hxx>

void timer2::init() {
  set_bit(ASSR, AS2);

  // stub

  loop_until_bit_is_clear(ASSR, TCR2BUB);
}
