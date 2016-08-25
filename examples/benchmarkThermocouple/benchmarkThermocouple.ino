#include <SPI.h>
#include <MAX31855_SPI.h>

byte cs = 2;

MAX31855_SPI * thermocouple;
uint32_t t, read_t, internal_t, both_t, count;

void setup() {
  thermocouple = new MAX31855_SPI(cs);

  Serial.begin(115200);
  SPI.begin();
}

void loop() {
  t = micros();
  thermocouple->readInternal();
  internal_t = micros() - t;

  t = micros();
  thermocouple->read();
  read_t = micros() - t;

  t = micros();
  thermocouple->readBoth();
  both_t = micros() - t;

  Serial.print(read_t);
  Serial.print(", ");
  Serial.print(internal_t);
  Serial.print(", ");
  Serial.println(both_t);

  delay(1000);
  count = 0;
  t = micros();

  while((micros() - t) < 1000000) {
    thermocouple->read();
    count = count + 1;
  }

  Serial.print(count);
  Serial.println(" read()/s");
}
