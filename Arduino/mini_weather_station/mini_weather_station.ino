#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>

#define SEALEVELPRESSURE_HPA (1013.25)

#define displayWidth 128
#define displayHeight 64

Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, &Wire);
Adafruit_BME280 bme; // I2C

unsigned long delayTime;
char temp[5];
char hum[5];
char pressure[4];

void setup() {
  Serial.begin(115200);

  unsigned status;

  status = bme.begin(0x76);
  // You can also pass in a Wire library object like &Wire2
  // status = bme.begin(0x76, &Wire2)
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
    Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(), 16);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");
    //while (1) delay(10);
  }

  display.begin(0x3C, true); // Address 0x3C default
  display.clearDisplay();
  display.display();
  display.setCursor(0, 10);
  display.setFont(&FreeSans9pt7b);
  //display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);

  
  display.println("Mini");
  display.println("Weather");
  display.println("Station");
  display.display();

  delay(1000);
  display.clearDisplay();
  display.display();


}

void loop() {
  printValues();
  delay(1000);

}

void printValues() {
  uint16_t x1, y1, x2, y2, cx1, cy1,cRadius;
  int16_t  sx1, sy1, sx2, sy2, scx1, scy1;
  uint16_t w1, h1, w2, h2, cw1, ch1;
  


  int p = bme.readPressure() / 100.0F;
  float t = bme.readTemperature() - 4;
  dtostrf(t, 5, 1, temp );
  dtostrf(p, 4, 0, pressure);
  dtostrf(bme.readHumidity(), 3, 0, hum);

  String line1 = String(temp) + " C";
  String line2 = String(hum) + "%  " + String(p) + "hPa";
  cRadius = 2;


  

  display.clearDisplay();
  
  display.setFont(&FreeSans12pt7b);
  display.getTextBounds(line1, 0, 0, &sx1, &sy1, &w1, &h1);
  display.getTextBounds(String(temp), 0, 0, &scx1, &scy1, &cw1, &ch1);
  
  x1 = (displayWidth - w1)/2;
  y1 = h1;
  cx1 = x1 + cw1 + cRadius + 2;
  cy1 = cRadius;
  
  display.setCursor(x1, y1);
  display.print(line1);
  
  display.drawCircle(cx1,cy1,cRadius, SH110X_WHITE);
  

  
    

  display.setFont(&FreeSans9pt7b);
  display.getTextBounds(line2, 0, 0, &sx2, &sy2, &w2, &h2);
  x2 = (displayWidth - w2)/2;
  y2 = y1 + 16 + h2;
  display.setCursor(x2, y2);
  display.print(line2);

  display.display();

  Serial.println("Line1: " + String(sx1) + ", " + String(sy1) + ", " + String(w1) + ", " + String(h1));
  Serial.println("Circle1: " + String(scx1) + ", " + String(scy1) + ", " + String(cw1) + ", " + String(ch1));
  Serial.println("Line2: " + String(sx2) + ", " + String(sy2) + ", " + String(w2) + ", " + String(h2));




  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.println(" °C");

  Serial.print("Pressure = ");

  Serial.print(bme.readPressure() / 100.0F);
  Serial.println(" hPa");

  Serial.print("Approx. Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");

  Serial.print("Humidity = ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");

  Serial.println();
}
