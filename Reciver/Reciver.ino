#include <Arduino.h>
#include <RF24.h>
#include <Wire.h>

RF24 myRadio(9, 10);

byte addresses[][6] = {"0"};
bool dataAreAvailable = false;

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

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  StartWirelessCommunication();
  delay(1000);
}

void loop()
{
  GetDataFromTransmitter();
  SendDataToMaster();
  // Serial.println("Temp. OUT: " + String(data.temperature));
  // Serial.println("Wilgotnosc OUT: " + String(data.humidity));
  // Serial.println("Cisnienie: " + String(data.pressure));
  // Serial.println("Temperatura BMP: " + String(data.temperatureBmp));
  // Serial.println("Errors: " + String(data.errors));
  // Serial.println("Czy sa dane: " + String(dataAreAvailable));
  delay(1000);
};

void StartWirelessCommunication()
{
  if (!myRadio.begin())
  {
    Serial.println("My Radio Error");
  }
  myRadio.setChannel(1);
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate(RF24_250KBPS);
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.startListening();
  delay(100);
};

void GetDataFromTransmitter()
{
  if (myRadio.available())
  {
    while (myRadio.available())
    {
      dataAreAvailable = true;
      myRadio.read(&data, sizeof(data));
    };
    // Serial.println("Dane dostępne.");
  }
  else
  {
    dataAreAvailable = false;
    // Serial.println("Dane niedostępne.");
  }
};

void SendDataToMaster() {
  if(dataAreAvailable)
  {
    String temp = String(data.temperature);
    String hum = String(data.humidity);
    String press = String((float)data.pressure/(float)100);

    String dataString = temp.substring(0, temp.length() - 1) 
      + ":" + String(data.temperatureBmp)
      + ":" + hum.substring(0, hum.length() - 1)
      + ":" + press.substring(0, press.length() - 1)
      + ":" + String(data.batteryPercent)
      + ":" + String(data.errors);

    // Serial.println("Wysyłanie danych: ");
    // Serial.println("Dane: " + dataToSend);
    Serial.write(dataString.c_str());
  }
}