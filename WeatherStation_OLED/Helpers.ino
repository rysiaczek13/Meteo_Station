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
};

String FormatTemp(float temp)
{
  String value = String(temp);
  return value.substring(0, value.length() - 1);
};