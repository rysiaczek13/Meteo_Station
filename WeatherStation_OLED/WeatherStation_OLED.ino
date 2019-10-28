#include <DHT.h>
#include <DS3231.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <Arduino.h>
#include <SPI.h>
#include <UTFT.h>
#include <URTouch.h>
#include <UTFT_Buttons.h>
#include <math.h>
#include <SDS011-select-serial.h>

#define DHTPIN 8
#define DHTTYPE DHT22

extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t Dingbats1_XL[];
extern uint8_t SevenSegNumFont[];

DS3231 clock;
RTCDateTime dt;
DHT dht(DHTPIN, DHTTYPE);
UTFT myGLCD(ILI9341_16, 38, 39, 40, 41);
URTouch myTouch(6,5,4,3,2);
UTFT_Buttons  myButtons(&myGLCD, &myTouch);
SDS011 my_sds(Serial2);

String dataString;
float minTempOut = (float)100.0;
float maxTempOut = (float)-100;
float minTempIn = (float)100;
float maxTempIn = (float)-100;
uint8_t actualDay = 0;
float p10,p25;
int error;
uint32_t lastSDSReadTime = 0;
uint32_t lastDataUpdateTime = 0;
uint32_t frequencyOfSDSReadings = 1800; //30 min
int x, y;

struct OutsideSensorsData
{
  float temperature;
  float humidity;
  float pressure;
  float temperatureBmp;
  int batteryPercent;
  int errors;
};

struct InsideSensorsData
{
  float insideTemperature;
  float insideHumidity;
};

struct MinMaxTempInside
{
  String minInTemp;
  String maxInTemp;
};

struct MinMaxTempOutside
{
  String minOutTemp;
  String maxOutTemp;
};

struct Time
{
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
  uint32_t unixtime;
  uint8_t day;
  uint8_t month;
  uint16_t year;
};

struct Smog
{
  String pm25Value;
  String pm10Value;
  bool newData;
};

int menuButton, pressedButton, timeSettingsButton, dateSettingsButton, smogSensorSettingsButton, closeMenuButton;
int hourUpButton, hourDownButton, minuteUpButton, minuteDownButton, saveButton, cancelButton;
int dayUpButton, dayDownButton, monthUpButton, monthDownButton, yearUpButton, yearDownButton;

void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial2.begin(9600);
  Wire.begin();
  dht.begin();
  
  if (!clock.begin())
  {
    Serial.println("RTC Error");
  };
  
  // clock.setDateTime(__DATE__, __TIME__);
  MainFrame();
};

void loop()
{
  if (myTouch.dataAvailable())
    {
      CheckClickedButton();
    }

  InsideSensorsData insidesensorsData = GetDataFromInsideSensors();
  Time timeData = GetTime();
  DisplayTime(timeData);
  DisplayDate(timeData);
  DisplayIndoorData(insidesensorsData);
  DisplayMinMaxTempInside(GetMinMaxTempInside(insidesensorsData));

  if (Serial1.available())
  {
    OutsideSensorsData outsideData =  GetDataFromReceiver(timeData.unixtime);
    DisplayOutdoorData(outsideData);
    DisplayMinMaxTempOutside(GetMinMaxTempOutside(outsideData));
    lastDataUpdateTime = GetTime().unixtime;
  }
  
  Smog smogData = GetSmog(timeData.unixtime);
  DisplaySmog(smogData);
  DisplayTheTimeToMeasureTheSmog(timeData.unixtime);
};