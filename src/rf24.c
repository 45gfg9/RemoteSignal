#include <teled.h>

// bad #define practice, don't follow
#define ADDR_A 0x17, 0x17, 0x07
#define ADDR_B 0x10, 0x18, 0x0B

#define DEVICE_ID 0

#if DEVICE_ID == 0
#define TX_ADDR ADDR_A
#define RX_ADDR ADDR_B
#else
#define TX_ADDR ADDR_B
#define RX_ADDR ADDR_A
#endif

#define CE PC1
#define CSN PC0
#define IRQ PD2

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

static void begin_transaction(void) {
  clear_bit(PORTC, CSN);
}

static void end_transaction(void) {
  set_bit(PORTC, CSN);
}

static uint8_t read(uint8_t addr) {
  begin_transaction();
  if (addr != REG_STATUS)
    spi_transfer(OP_R_REGISTER | addr);
  uint8_t result = spi_transfer(OP_NOP);
  end_transaction();

  return result;
}

static uint8_t write_cmd(uint8_t cmd) {
  begin_transaction();
  uint8_t status = spi_transfer(cmd);
  end_transaction();

  return status;
}

static uint8_t write_reg(uint8_t addr, uint8_t val, uint8_t mask) {
  begin_transaction();
  uint8_t status = spi_transfer(mask | addr);
  spi_transfer(val);
  end_transaction();

  return status;
}

static uint8_t write_addr(uint8_t addr, uint8_t b0, uint8_t b1, uint8_t b2) {
  begin_transaction();
  uint8_t status = spi_transfer(OP_W_REGISTER | addr);
  spi_transfer(b0);
  spi_transfer(b1);
  spi_transfer(b2);
  end_transaction();

  return status;
}

static uint8_t reset_irq(void) {
  return write_reg(REG_STATUS, 0x70, OP_W_REGISTER);
}

void rf24_init(void) {
  set_bit(PORTC, CSN);
  set_bit(DDRC, CSN);
  set_bit(DDRC, CE);

  spi_begin();

  _delay_ms(100);

  // enable all interrupts
  // CRC 1B
  write_reg(REG_CONFIG, 0b1001, OP_W_REGISTER);

  // set pipe 0 & 1 auto ack
  write_reg(REG_EN_AA, 0b11, OP_W_REGISTER);

  // set address width 3B
  write_reg(REG_SETUP_AW, 0b01, OP_W_REGISTER);

  // set 15 retries
  write_reg(REG_SETUP_RETR, 0b1111, OP_W_REGISTER);

  // set channel
  write_reg(REG_RF_CH, 111, OP_W_REGISTER);

  // set PA level -6dBm
  // set data rate 2Mbps
  write_reg(REG_RF_SETUP, 0b001100, OP_W_REGISTER);

  // set TX address
  write_addr(REG_RX_ADDR_P0, TX_ADDR);
  write_addr(REG_TX_ADDR, TX_ADDR);

  // set RX address
  write_addr(REG_RX_ADDR_P1, RX_ADDR);

  // set payload size 1B
  write_reg(REG_RX_PW_P0, 1, OP_W_REGISTER);
  write_reg(REG_RX_PW_P1, 1, OP_W_REGISTER);

  spi_end();
}

void rf24_begin(void) {
  spi_begin();

  write_reg(REG_CONFIG, 0b1011, OP_W_REGISTER); // power on
}

void rf24_end(void) {
  write_reg(REG_CONFIG, 0b1001, OP_W_REGISTER); // power off

  spi_end();
}

bool rf24_rx(uint8_t *payload) {
  set_bit(PORTC, CE);
  _delay_us(130); // RX Setting 130µs

  for (uint8_t i = 200; i; i--) {
    _delay_us(10);
    if (bit_is_clear(PIND, IRQ))
      break;
  }

  clear_bit(PORTC, CE);

  uint8_t status = reset_irq(); // clear IRQ pin

  bool rx_dr = status & _BV(6); // RX Data Ready interrupt
  if (rx_dr && payload)
    *payload = read(OP_R_RX_PAYLOAD);
  else
    write_cmd(OP_FLUSH_RX);

  return rx_dr;
}

bool rf24_tx(uint8_t payload) {
  write_reg(REG_CONFIG, 0b1010, OP_W_REGISTER); // PRIM_RX = 0

  write_reg(OP_W_TX_PAYLOAD, payload, 0);

  set_bit(PORTC, CE);
  uint8_t timeout = 0;
  while (bit_is_set(PORTD, IRQ) && --timeout)
    _delay_us(10);
  clear_bit(PORTC, CE);

  uint8_t status = reset_irq(); // clear IRQ pin

  write_cmd(OP_FLUSH_TX);
  write_reg(REG_CONFIG, 0b1011, OP_W_REGISTER); // PRIM_RX = 1

  return status & _BV(5); // TX Data Sent interrupt
}
