#include <Teled.hxx>

static const auto CE = PC1;
static const auto CSN = PC0;
static const auto IRQ = PD2;

enum ops {
  OP_R_REGISTER = 0x00, // mask
  OP_W_REGISTER = 0x20, // mask
  OP_R_RX_PAYLOAD = 0x61,
  OP_W_TX_PAYLOAD = 0xA0,
  OP_FLUSH_TX = 0xE1,
  OP_FLUSH_RX = 0xE2,
  OP_REUSE_TX_PL = 0xE3,
  OP_RX_PL_WID = 0x60,
  OP_W_ACK_PAYLOAD = 0xA8, // mask
  OP_W_TX_PAYLOAD_NOACK = 0xB0,
  OP_NOP = 0xFF,
};

enum regs {
  REG_CONFIG = 0x00,
  REG_EN_AA = 0x01,
  REG_EN_RXADDR = 0x02,
  REG_SETUP_AW = 0x03,
  REG_SETUP_RETR = 0x04,
  REG_RF_CH = 0x05,
  REG_RF_SETUP = 0x06,
  REG_STATUS = 0x07,
  REG_OBSERVE_TX = 0x08,
  REG_RPD = 0x09,
  REG_RX_ADDR_P0 = 0x0A,
  REG_RX_ADDR_P1 = 0x0B,
  REG_RX_ADDR_P2 = 0x0C,
  REG_RX_ADDR_P3 = 0x0D,
  REG_RX_ADDR_P4 = 0x0E,
  REG_RX_ADDR_P5 = 0x0F,
  REG_TX_ADDR = 0x10,
  REG_RX_PW_P0 = 0x11,
  REG_RX_PW_P1 = 0x12,
  REG_RX_PW_P2 = 0x13,
  REG_RX_PW_P3 = 0x14,
  REG_RX_PW_P4 = 0x15,
  REG_RX_PW_P5 = 0x16,
  REG_FIFO_STATUS = 0x17,
  REG_DYNPD = 0x1C,
  REG_FEATURE = 0x1D,
};

static void begin_transaction() {
  clear_bit(PORTC, CSN);
}

static void end_transaction() {
  set_bit(PORTC, CSN);
}

void rf24::begin() {
  spi::begin();
}

void rf24::end() {
  spi::end();
}

uint8_t rf24::read_reg(uint8_t addr) {
  begin_transaction();
  spi::transfer(OP_R_REGISTER | addr);
  auto result = spi::transfer(OP_NOP);
  end_transaction();

  return result;
}

uint8_t rf24::write_reg(uint8_t addr, uint8_t val) {
  begin_transaction();
  auto status = spi::transfer(OP_W_REGISTER | addr);
  spi::transfer(val);
  end_transaction();

  return status;
}

uint8_t rf24::write_reg(uint8_t addr, const uint8_t *buf, uint8_t len) {
  begin_transaction();
  auto status = spi::transfer(OP_W_REGISTER | addr);
  while (len--)
    spi::transfer(*buf++);
  end_transaction();

  return status;
}

bool rf24::available() {
  for (uint8_t i = 6; i; i--) {
    _delay_us(500);
    if (bit_is_clear(PIND, IRQ)) {
      // TODO clear interrupt flag
      return true;
    }
  }
  return false;
}

uint8_t rf24::rx() {
  // TODO
  return 0;
}

bool rf24::tx(uint8_t payload) {
  // TODO
  return false;
}
