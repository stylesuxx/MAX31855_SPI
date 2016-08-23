#include "MAX31855_SPI.h"

MAX31855_SPI::MAX31855_SPI(byte cs) {
  _cs = cs;

  _spiSettings = SPISettings(20000000, MSBFIRST, SPI_MODE0);
  pinMode(_cs, OUTPUT);
  digitalWrite(_cs, HIGH);
}

double MAX31855_SPI::readInternal() {
  long value = spiRead();

  /* Shift away thermo couple data. */
  value >>= 4;

  /* We are only interested in 12 Bit internal temp data (last 12 Bit).*/
  float internal = value & 0x07FF;

  /* Convert to negative number if sign bit set */
  if(value & 0x0800) {
    int tmp = 0xF800 | (value & 0x07FF);
    internal = tmp;
  }

  internal *= 0.0625;

  return internal;
}

double MAX31855_SPI::read() {
  long value = spiRead();

  /* Check if fault bit is set. */
  if(value & 0x7) {
    return NAN;
  }

  /* Shift away internal data and error bits. */
  value >>= 18;

  /* 14 Bits are left */
  if(value & 0x2000) {
      value = 0xFFFFC000 | (value & 0x0003FFFF);
  }

  double temp = value * 0.25;

  return temp;
}

long MAX31855_SPI::spiRead() {
  int i;

  union bytes_to_uint32 {
      byte bytes[4];
      long integer;
  } buffer;

  SPI.beginTransaction(_spiSettings);
  digitalWrite(_cs, LOW);

  for(i=3; i>=0; i--) {
    buffer.bytes[i] = SPI.transfer(0x00);
  }

  digitalWrite(_cs, HIGH);
  SPI.endTransaction();

  return buffer.integer;
}
