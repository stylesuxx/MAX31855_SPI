#include <SPI.h>
#include "MAX31855_SPI.h"

byte cs = 2;
MAX31855_SPI *max31855;

void setup() {
  max31855 = new MAX31855_SPI(cs);

  Serial.begin(115200);
  SPI.begin();
}

void loop() {
  double internal = max31855->readInternal();
  double temp = max31855->read();

  Serial.print("Internal: ");
  Serial.println(internal);

  Serial.println(temp);
}
