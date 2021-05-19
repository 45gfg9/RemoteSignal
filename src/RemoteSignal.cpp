#include <RemoteSignal.hxx>

int main() {
  init_io();

  for (;;) {
    loop();
  }

  return 0;
}
