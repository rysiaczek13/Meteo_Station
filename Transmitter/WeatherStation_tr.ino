#include <RF24.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <DHT.h>
#include <Wire.h>
#include <SPI.h>
#include <Arduino.h>
#include <LowPower.h>

#define DHTPIN 3
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
RF24 myRadio (9, 10);
Adafruit_BMP280 bmp;

byte addresses[][6] = {"0"};
struct package
{
  float temperature;
  float humidity;
  float pressure;
  float temperatureBmp;
  byte batteryPercent;
  byte errors;
};

typedef struct package Package;
Package data;

void setup() {
  Serial.begin(115200);
  dht.begin();
  if(!bmp.begin(0x76))
  {
    Serial.println("Nie odnaleziono czujnika BMP280");
    data.errors = 1;
  }
  Wire.begin();
  startWirelessCommunication();
  delay(1000);
}

void loop() {
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  readSensors();
  myRadio.write(&data, sizeof(data)); 
  delay(1000);
}

void readSensors() {
  Serial.println("Read sensors");
  dht.read(true);
  data.temperature = dht.readTemperature();
  data.humidity = dht.readHumidity();
  data.pressure = bmp.readPressure();
  data.temperatureBmp = bmp.readTemperature();
  data.batteryPercent = 100;
  
  Serial.print("Temperatura: ");
  Serial.println(data.temperature);
  Serial.print("Wilgotnosc: ");
  Serial.println(data.humidity);
  Serial.print("TemperaturaBMP: ");
  Serial.println(data.temperatureBmp);
  Serial.print("Cisnienie: ");
  Serial.println(data.pressure);
  Serial.print("Battery Percent: ");
  Serial.println(data.batteryPercent);
  Serial.print("Errors: ");
  Serial.println(data.errors);
}

void startWirelessCommunication()
{
  if (!myRadio.begin())
  {
    Serial.println("My Radio Error");
    data.errors = 2;
  } 
  myRadio.setChannel(1); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS ) ; 
  myRadio.openWritingPipe( addresses[0]);
  delay(100);
};
