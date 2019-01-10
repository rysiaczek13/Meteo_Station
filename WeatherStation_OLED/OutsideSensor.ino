OutsideSensorsData GetDataFromReceiver(uint32_t actualUnixTime)
{
  OutsideSensorsData outsideData;
  dataString = Serial1.readString();

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

  return outsideData;
};

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

void DisplayOutdoorData(OutsideSensorsData sensorsData)
{
    String temp = String(sensorsData.temperature).substring(0, String(sensorsData.temperature).length() - 1);
    String hum = String(sensorsData.humidity).substring(0, String(sensorsData.humidity).length() - 1);
    String press = String(sensorsData.pressure).substring(0, String(sensorsData.pressure).length() - 1);

    myGLCD.setColor(102, 204, 255);
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

void DisplayMinMaxTempOutside(MinMaxTempOutside temp)
{
  myGLCD.setColor(255, 255, 255);
  myGLCD.setFont(SmallFont);
  myGLCD.print("OUT", 225, 40);
  myGLCD.setColor(0, 0, 255); //min
  myGLCD.print("    ", 215, 60); //To clear the value on screen
  myGLCD.print(temp.minOutTemp, 220, 60); //OUT
  myGLCD.setColor(255, 0, 0); //max
  myGLCD.print("    ", 215, 80); //To clear the value on screen
  myGLCD.print(temp.maxOutTemp, 220, 80); //OUT 
}