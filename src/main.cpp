#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Wire.h>
#include <TFT_eSPI.h>
#include "ScioSense_ENS160.h"

ScioSense_ENS160 ens160(0x53);
Adafruit_BME280 bme;
TFT_eSPI tft = TFT_eSPI();

#define LED1_PIN 35
#define LED2_PIN 36
#define LED3_PIN 37
#define LED4_PIN 38
#define LED5_PIN 39

const int photoResistorPin = 15;
int sensorValue = 0;

void turnOffAllLEDs()
{
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);
  digitalWrite(LED4_PIN, LOW);
  digitalWrite(LED5_PIN, LOW);
}

void controlLEDs(int aqi)
{
  // Wyłącz wszystkie diody na początku
  turnOffAllLEDs();

  // Logika sterowania LED
  if (aqi > 0)
    digitalWrite(LED1_PIN, HIGH); // TVOC > 0
  if (aqi > 1)
    digitalWrite(LED2_PIN, HIGH); // TVOC > 100
  if (aqi > 2)
    digitalWrite(LED3_PIN, HIGH); // TVOC > 200
  if (aqi > 3)
    digitalWrite(LED4_PIN, HIGH); // TVOC > 300
  if (aqi > 4)
    digitalWrite(LED5_PIN, HIGH); // TVOC > 400
}

void setup() {
Wire.begin(8,9);
Serial.begin(115200);
bme.begin(0x76);
tft.init();
tft.setRotation(1);
tft.setTextSize(2);
tft.setTextColor(TFT_WHITE, TFT_BLACK);
Serial.print("ENS160...");
  bool ok = ens160.begin();
  Serial.println(ens160.available() ? "done." : "failed!");
  if (ens160.available())
  {
    // Print ENS160 versions
    Serial.print("\tRev: ");
    Serial.print(ens160.getMajorRev());
    Serial.print(".");
    Serial.print(ens160.getMinorRev());
    Serial.print(".");
    Serial.println(ens160.getBuild());

    Serial.print("\tCustom mode ");
    ens160.initCustomMode(3); 
    Serial.println(ens160.setMode(ENS160_OPMODE_STD) ? "done." : "failed!");
  }
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(LED4_PIN, OUTPUT);
  pinMode(LED5_PIN, OUTPUT);
}


void loop() {

  float temp = bme.readTemperature();
  Serial.print("Temperature = ");
  Serial.print(temp);
  Serial.println(" *C");
  
  float press = bme.readPressure() / 100.0F;
  Serial.print("Pressure = ");
  Serial.print(press);
  Serial.println(" hPa");
  
  float hum = bme.readHumidity();
  Serial.print("Humidity = ");
  Serial.print(hum);
  Serial.println(" %");

  ens160.measure(true);
  Serial.print("AQI: ");
  float aqi = ens160.getAQI();
  Serial.print(aqi);
  Serial.println(" units\t");
  controlLEDs(ens160.getAQI());
  

  int rawValue = analogRead(photoResistorPin);
  float voltage = (4095-rawValue) * (3.3/4095);     
  float amps = voltage / 10000.0;
  float microamps = amps * 1000000;
  float lux = microamps * 2.0; 
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lux");

  Serial.println();

  tft.fillScreen(TFT_BLACK);
  tft.setCursor(10, 10); 
  tft.print("Temperature: ");
  tft.print(temp);
  tft.println(" *C");

  tft.setCursor(10, 40);
  tft.print("Pressure: ");
  tft.print(press);
  tft.println(" hPa");

  tft.setCursor(10, 70);
  tft.print("Humidity: ");
  tft.print(hum);
  tft.println(" %");

  tft.setCursor(10, 100);
  tft.print("AQI: ");
  tft.print(aqi);
  tft.println(" units");

  tft.setCursor(10, 130);
  tft.print("Light: ");
  tft.print(lux);
  tft.println(" lux");

  delay(2000);
}
