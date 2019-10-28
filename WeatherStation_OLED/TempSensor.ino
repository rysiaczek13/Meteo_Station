InsideSensorsData GetDataFromInsideSensors()
{
  dht.read(true);
  delay(500);
  InsideSensorsData data;
  data.insideTemperature = dht.readTemperature();
  data.insideHumidity = round(dht.readHumidity());

  return data;
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

void DisplayIndoorData(InsideSensorsData sensorsData)
{
  String temp = String(sensorsData.insideTemperature).substring(0, String(sensorsData.insideTemperature).length() - 1);
  String hum = String(sensorsData.insideHumidity).substring(0, String(sensorsData.insideHumidity).length() - 1);

  myGLCD.setColor(102, 255, 51);
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