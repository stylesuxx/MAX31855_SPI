#ifndef MAX31855_SPI_H
#define MAX31855_SPI_H

#include <Arduino.h>
#include <SPI.h>

struct temperatures_t {
  double internal;
  double temp;
};

class MAX31855_SPI {
  public:
    explicit MAX31855_SPI(byte cs);

    /* Returns internal temperature in degrees celsius. */
    double readInternal();

    /* Returns temperature in degrees celsius. */
    double read();

    /* Returns a temperatures struct containing internal and measured
     * temperature
     */
    temperatures_t readBoth();

  private:
    uint32_t spiRead();
    double getInternal(uint32_t value);
    double getTemp(uint32_t value);

    uint8_t _cs;
    double _multiplierInternal;
    double _multiplierTemp;
    SPISettings _spiSettings;
};

#endif
