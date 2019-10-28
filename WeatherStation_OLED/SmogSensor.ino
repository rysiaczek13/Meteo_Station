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
      lastSDSReadTime = actualUnixTime;
      my_sds.sleep();
    }
  }
  return smog;
};

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