#ifndef MAX31855_SPI_H
#define MAX31855_SPI_H

#include <Arduino.h>
#include <SPI.h>

class MAX31855_SPI {
  public:
    explicit MAX31855_SPI(byte cs);

    /* Returns internal temperature in degrees celsius. */
    double readInternal();

    /* Returns temperature in degrees celsius. */
    double read();

  private:
    uint32_t spiRead();

    uint8_t _cs;
    SPISettings _spiSettings;
};

#endif
