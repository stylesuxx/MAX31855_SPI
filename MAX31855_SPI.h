#ifndef MAX31855_SPI_H
#define MAX31855_SPI_H

#include <Arduino.h>
#include <SPI.h>

class MAX31855_SPI {
  public:
    explicit MAX31855_SPI(byte cs);

    double readInternal();
    double read();

  private:
    long spiRead();

    byte _cs;
    SPISettings _spiSettings;
};

#endif
