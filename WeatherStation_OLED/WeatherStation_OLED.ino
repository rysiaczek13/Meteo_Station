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
  byte hour;
  byte minute;
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

void setup()
{
  int menuButton, but2, but3, but4, butX, butY, pressed_button;
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial2.begin(9600);
  Wire.begin();
  myGLCD.setBrightness(16);
  myGLCD.InitLCD();
  myGLCD.clrScr();
  myGLCD.setFont(SmallFont);
  myTouch.InitTouch();
  myTouch.setPrecision(PREC_MEDIUM);
  myButtons.setTextFont(SmallFont);
  DrawHome();
  menuButton = myButtons.addButton( 260, 30, 50,  30, "MENU");
  myButtons.drawButtons();
  if (!clock.begin())
  {
    Serial.println("RTC Error");
  };
  dht.begin();
  // clock.setDateTime(__DATE__, __TIME__);
};

void loop()
{
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
  
  // Smog smogData = GetSmog(timeData.unixtime);
  // DisplaySmog(smogData);
  DisplayTheTimeToMeasureTheSmog(timeData.unixtime);
  // Serial.println("Data: " + GetMinuts(timeData.day) + "-" + GetMinuts(timeData.month) + "-" + String(timeData.year));
  // Serial.println("Godzina: " + GetMinuts(timeData.hour) + ":" + GetMinuts(timeData.minute));
  // Serial.println("Temp. IN: " + String(insidesensorsData.insideTemperature));
  // Serial.println("Wilgotnosc IN: " + String(insidesensorsData.insideHumidity));
  // Serial.println("Temp. OUT: " + String(outsideData.temperature));
  // Serial.println("Wilgotnosc OUT: " + String(outsideData.humidity));
  // Serial.println("Cisnienie: " + String(outsideData.pressure));
  // Serial.println("Temperatura BMP: " + String(outsideData.temperatureBmp));
  // Serial.println("Battery " + String(outsideData.batteryPercent));
  // Serial.println("Errors: " + String(outsideData.errors));
  // Serial.println(String(timeData.unixtime) + ":" + String(lastSDSReadTime) + ":" + String(frequencyOfSDSReadings));
  // Serial.println(String(timeData.unixtime - lastSDSReadTime));
  // if(timeData.unixtime - lastDataUpdateTime >= 300)
  // {
  //   DisplayDataAllert();
  // }
};

Time GetTime()
{
  dt = clock.getDateTime();
  Time time;
  time.hour = dt.hour;
  time.minute = dt.minute;
  time.second = dt.second;
  time.unixtime = dt.unixtime;
  time.day = dt.day;
  time.month = dt.month;
  time.year = dt.year;
  return time;
}

String GetMinuts(byte minutes)
{
  if (minutes < 10)
  {
    return "0" + String(minutes);
  }
  return String(minutes);
};

OutsideSensorsData GetDataFromReceiver(uint32_t actualUnixTime)
{
  OutsideSensorsData outsideData;
  dataString = Serial1.readString();

  //Serial.println("dane z portu " + dataString);

  int delimiter1 = dataString.indexOf(":");
  int delimiter2 = dataString.indexOf(":", delimiter1 + 1);
  int delimiter3 = dataString.indexOf(":", delimiter2 + 1);
  int delimiter4 = dataString.indexOf(":", delimiter3 + 1);
  int delimiter5 = dataString.indexOf(":", delimiter4 + 1);

  int length = dataString.length();

  outsideData.temperature =  dataString.substring(0,delimiter1).toFloat();
  outsideData.temperatureBmp = dataString.substring(delimiter1 + 1, delimiter2).toFloat();
  outsideData.humidity = dataString.substring(delimiter2 + 1, delimiter3).toFloat();
  outsideData.pressure = dataString.substring(delimiter3 + 1, delimiter4).toFloat();
  outsideData.batteryPercent = dataString.substring(delimiter4 + 1, delimiter5).toInt();
  outsideData.errors = dataString.substring(delimiter5 + 1, length).toInt();

  dataString = "";

  // Serial.println("Temp. OUT: " + dataString.substring(0,delimiter1));
  // Serial.println("Temp. BMP: " + dataString.substring(delimiter1, delimiter2));
  // Serial.println("Hum. OUT: " + dataString.substring(delimiter2, delimiter3));
  // Serial.println("Pressure: " + dataString.substring(delimiter3, delimiter4));
  // Serial.println("Battery: " + dataString.substring(delimiter4, delimiter5));
  // Serial.println("Errors: " + dataString.substring(delimiter5, length));

  return outsideData;
};

void DrawHome()
{
  myGLCD.setColor(0, 190, 0);
  myGLCD.fillRect(20, 175, 150, 180);
  myGLCD.fillRect(20, 175, 25, 125);
  myGLCD.fillRect(145, 175, 150, 125);

  for (int i = 0; i <= 5; i++)
  {
    myGLCD.drawLine(85, 65 - i, 160, 140 - i);
    myGLCD.drawLine(85, 65 - i, 10, 140 - i);
  }
};

void DisplayIndoorData(InsideSensorsData sensorsData)
{
  String temp = String(sensorsData.insideTemperature).substring(0, String(sensorsData.insideTemperature).length() - 1);
  String hum = String(sensorsData.insideHumidity).substring(0, String(sensorsData.insideHumidity).length() - 1);

  myGLCD.setColor(255, 255, 255);
  myGLCD.setFont(BigFont);
  myGLCD.print(temp, 45, 125, 0);
  myGLCD.setFont(SmallFont);
  myGLCD.print("o", 116, 120);
  myGLCD.setFont(BigFont);
  myGLCD.print("C", 121, 125);
  myGLCD.print(hum + String("%"), 45, 155);

  temp = "";
  hum = "";
}

void DisplayOutdoorData(OutsideSensorsData sensorsData)
{
    String temp = String(sensorsData.temperature).substring(0, String(sensorsData.temperature).length() - 1);
    String hum = String(sensorsData.humidity).substring(0, String(sensorsData.humidity).length() - 1);
    String press = String(sensorsData.pressure).substring(0, String(sensorsData.pressure).length() - 1);

    myGLCD.setColor(255, 255, 255);
    myGLCD.setFont(BigFont);
    myGLCD.print("          ", RIGHT, 105);
    myGLCD.print(press + " hPa", RIGHT, 105);
    myGLCD.print("            ", 190, 135, 0);
     myGLCD.print(temp, 200, 135, 0);
    myGLCD.setFont(SmallFont);
    myGLCD.print("o", 270, 130);
    myGLCD.setFont(BigFont);
    myGLCD.print("C", 275, 135);                         
    myGLCD.print("           ", 190, 165);
    myGLCD.print(hum + String("%"), 200, 165);

    temp = "";
    hum = "";
    press = "";
}

void DisplayTime(Time time)
{
  myGLCD.setColor(255, 255, 255);
  myGLCD.setFont(SevenSegNumFont);
  myGLCD.print(GetMinuts(time.hour), 5, 10);
  myGLCD.print(GetMinuts(time.minute), 80, 10);
  myGLCD.fillCircle(73, 25, 4);
  myGLCD.fillCircle(73, 45, 4);
}

void DisplayDate(Time time)
{
  myGLCD.setColor(255, 255, 255);
  myGLCD.setFont(BigFont);
  String date = GetMinuts(time.day) + "-" + GetMinuts(time.month) + "-" + String(time.year);
  myGLCD.print(date, 150, 10, 0);
}

void DisplayMinMaxTempInside(MinMaxTempInside temp)
{
  myGLCD.setColor(255, 255, 255);
  myGLCD.setFont(SmallFont);
  myGLCD.print("IN", 165, 40);
  myGLCD.setColor(0, 0, 255); //min
  myGLCD.print(temp.minInTemp, 160, 60); //IN
  myGLCD.setColor(255, 0, 0); //max
  myGLCD.print(temp.maxInTemp, 160, 80); //IN
}

void DisplayMinMaxTempOutside(MinMaxTempOutside temp)
{
  myGLCD.setColor(255, 255, 255);
  myGLCD.setFont(SmallFont);
  myGLCD.print("OUT", 225, 40);
  myGLCD.setColor(0, 0, 255); //min
  myGLCD.print("          ", 215, 60); //To clear the value on screen
  myGLCD.print(temp.minOutTemp, 220, 60); //OUT
  myGLCD.setColor(255, 0, 0); //max
  myGLCD.print("          ", 215, 80); //To clear the value on screen
  myGLCD.print(temp.maxOutTemp, 220, 80); //OUT 
}

void DisplaySmog(Smog smog)
{
  if(smog.newData)
  {
    myGLCD.setColor(VGA_AQUA);
    myGLCD.setFont(BigFont);
    myGLCD.print("                             ", LEFT, 195);
    myGLCD.print("                             ", LEFT, 220);
    myGLCD.print(smog.pm25Value, LEFT, 195);
    myGLCD.print(smog.pm10Value, LEFT, 220);
  }
}

void DisplayDataAllert()
{
    myGLCD.setColor(VGA_RED);
    myGLCD.fillRoundRect(5,120,15,110);
}

void DisplayTheTimeToMeasureTheSmog(uint32_t actualUnixTime)
{ 
  uint32_t timeFromLastMeasure = actualUnixTime - lastSDSReadTime;
  uint32_t timeToMeasureTheSmog = frequencyOfSDSReadings - timeFromLastMeasure;

  myGLCD.setColor(VGA_BLACK);
  myGLCD.setFont(SmallFont);
  myGLCD.setBackColor(VGA_YELLOW);
  myGLCD.print("     ",5,66);
  myGLCD.print(String(timeToMeasureTheSmog),5,66);
  myGLCD.setBackColor(VGA_BLACK);
};

MinMaxTempInside GetMinMaxTempInside(InsideSensorsData dataIn)
{
  if (dataIn.insideTemperature < minTempIn)
  {
    minTempIn = dataIn.insideTemperature;
  }

  if (dataIn.insideTemperature > maxTempIn)
  {
    maxTempIn = dataIn.insideTemperature;
  }

  MinMaxTempInside temp;

  temp.maxInTemp = FormatTemp(maxTempIn);
  temp.minInTemp = FormatTemp(minTempIn);

  return temp;
}

MinMaxTempOutside GetMinMaxTempOutside(OutsideSensorsData dataOut)
{
  if (dataOut.temperature < minTempOut)
  {
    minTempOut = dataOut.temperature;
  }

  if (dataOut.temperature > maxTempOut)
  {
    maxTempOut = dataOut.temperature;
  }

  MinMaxTempOutside temp;

  temp.maxOutTemp = FormatTemp(maxTempOut);
  temp.minOutTemp = FormatTemp(minTempOut);

  return temp;
}

void ResetMinMaxTemp(Time time)
{
  if(actualDay != time.day)
  {
    minTempIn = 100;
    maxTempIn = -100;
    minTempOut = 100;
    maxTempOut = -100;
  }
  actualDay = time.day;
}

String FormatTemp(float temp)
{
  String value = String(temp);
  return value.substring(0, value.length() - 1);
}

InsideSensorsData GetDataFromInsideSensors()
{
  dht.read(true);
  delay(1000);
  InsideSensorsData data;
  data.insideTemperature = dht.readTemperature();
  data.insideHumidity = round(dht.readHumidity());

  return data;
};

Smog GetSmog(uint32_t actualUnixTime)
{
  int var = 0;
  Smog smog;
  smog.newData = false;
  if (actualUnixTime - lastSDSReadTime >= frequencyOfSDSReadings)
  {
    my_sds.wakeup();

    myGLCD.setColor(VGA_BLACK);
    myGLCD.setFont(SmallFont);
    myGLCD.setBackColor(VGA_YELLOW);

    while(var < 30){      
      myGLCD.print("     ", 5, 66);
      myGLCD.print(String(30 - var), 5, 66);
      var++;
      delay(1000);
    };

    myGLCD.setBackColor(VGA_BLACK);
    error = my_sds.read(&p25,&p10);
    if (!error) {
      int percent25 = (int)(p25 / 25.0 * 100.0);
      int percent10 = (int)(p10 / 50.0 * 100.0);

      smog.pm25Value = "PM2.5: " + String((int)p25) + "ug " + String(percent25) + "%";
      smog.pm10Value = "PM10:  " + String((int)p10) + "ug " + String(percent10) + "%";
      smog.newData = true;
      // Serial.println("P2.5: "+String(p25));
      // Serial.println("P10:  "+String(p10));
      // Serial.println(smog.pm25Value);
      // Serial.println(smog.pm10Value);
      lastSDSReadTime = actualUnixTime;
      my_sds.sleep();
    }
    // Serial.println("ErrorSDS: " + String(error));
  }
  return smog;
};