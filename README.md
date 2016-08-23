# MAX31855 Arduino SPI library
This is an **SPI only** library implementation for the MAX31855. This will only work on platforms with hardware SPI.

The motivation behind this library is to keep it as small, fast and straight forward as possible.

This library enables you to read internal and measured temperature in degrees celsius via **SPI**.

## Usage
An example is provided in the examples directory. It will print internal and measured temperature to the serial console.

Pin 2 is used for the cable select pin.
Clk needs to be connected to the SPI clock.
D0 needs to be connected to SPI MISO.

```Arduino
Oversample * sampler;

byte cs = 2;
MAX31855_SPI * max31855;

void setup() {
  max31855 = new MAX31855_SPI(cs);

  Serial.begin(115200);
  SPI.begin();
}

void loop() {
  double internal = max31855->readInternal();
  double temp = max31855->read();

  Serial.println(temp, 2);
}
```
