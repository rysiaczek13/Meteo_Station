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