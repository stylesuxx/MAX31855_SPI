#include <SPI.h>
#include <MAX31855_SPI.h>

/* https://github.com/sumotoy/SSD_13XX */
#include <SSD_13XX.h>
#include "_fonts/mono_mid.c"

#define	BLACK 0x0000
#define WHITE 0xFFFF

double internal, external, lastInternal, lastExternal;

/* Using SPI for display and thermocoupler, those are the additional wires needed to connect. */
byte csSsd = 3;
byte dcSsd = 8;

byte csMax = 2;

SSD_13XX * display; //= SSD_13XX(csSsd, dcSsd);
MAX31855_SPI * thermocouple;

void setup() {
  display = new SSD_13XX(csSsd, dcSsd);
  thermocouple = new MAX31855_SPI(csMax);
  lastInternal = 0;
  lastExternal = 0;

  Serial.begin(115200);
  SPI.begin();

  display->begin();
  display->setCursor(0,0);
  display->fillScreen(BLACK);
  display->setFont(&mono_mid);
  display->setTextColor(WHITE, BLACK);
}

void loop() {
  temperatures_t temps = thermocouple->readBoth();
  internal = temps.internal;
  external = temps.temp;

  if (isnan(external)) {
    display->fillScreen(BLACK);
    display->setCursor(0, 0);
    display->print("Something wrong with thermocouple!");
    return;
  }

  if(internal != lastInternal) {
    display->setCursor(13, 17);
    renderInternal(internal);
    lastInternal = internal;
  }

  if(external != lastExternal) {
    display->setCursor(5, 37);
    renderExternal(external);
    lastExternal = external;
  }

  delay(500);
}

void renderExternal(double temp) {
  String padding = "";
  if(temp < 100) padding += " ";
  if(temp < 10) padding += " ";

  display->setTextScale(2);
  display->print(padding);
  display->print(temp, 2);
  display->println("C ");
  display->setTextScale(1);
}

void renderInternal(double temp) {
  display->print("Int: ");
  display->print(temp, 2);
  display->println("C");
}
