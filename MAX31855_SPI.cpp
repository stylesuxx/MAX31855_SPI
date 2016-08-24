#include "MAX31855_SPI.h"

MAX31855_SPI::MAX31855_SPI(byte cs) {
  _cs = cs;

  _spiSettings = SPISettings(20000000, MSBFIRST, SPI_MODE0);
  _multiplierInternal = 0.25D;
  _multiplierTemp =  0.0625D;

  pinMode(_cs, OUTPUT);
  digitalWrite(_cs, HIGH);
}

double MAX31855_SPI::readInternal() {
  uint32_t value = spiRead();
  return getInternal(value);
}

double MAX31855_SPI::read() {
  uint32_t value = spiRead();
  return getTemp(value);
}

temperatures_t MAX31855_SPI::readBoth() {
  uint32_t value = spiRead();
  temperatures_t temps;

  temps.internal = getInternal(value);
  temps.temp = getTemp(value);

  return temps;
}

double MAX31855_SPI::getTemp(uint32_t value) {
  /* Check if fault bit is set. */
  if(value & 0x7) {
    return NAN;
  }

  /* Shift away internal data and error bits. */
  value >>= 18;

  /* 14 Bits are left, check if negative number */
  if(value & 0x2000) {
    value = 0xFFFFC000 | (value & 0x0003FFFF);
  }

  double temp = value * _multiplierInternal;
  return temp;
}

double MAX31855_SPI::getInternal(uint32_t value) {
  /* Shift away thermo couple data. */
  value >>= 4;

  /* We are only interested in 12 Bit internal temp data (last 12 Bit).*/
  double internal = value & 0x07FF;

  /* Convert to negative number if sign bit set */
  if(value & 0x0800) {
    int tmp = 0xF800 | (value & 0x07FF);
    internal = tmp;
  }

  internal *= _multiplierTemp;
  return internal;
}

uint32_t MAX31855_SPI::spiRead() {
  uint8_t i;

  union bytes_to_uint32 {
    uint8_t bytes[4];
    uint32_t integer;
  } buffer;

  SPI.beginTransaction(_spiSettings);
  digitalWrite(_cs, LOW);

  for(i = 4; i > 0; i--) {
    buffer.bytes[i - 1] = SPI.transfer(0x00);
  }

  digitalWrite(_cs, HIGH);
  SPI.endTransaction();

  return buffer.integer;
}
