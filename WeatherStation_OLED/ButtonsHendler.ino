void CheckClickedButton()
{
  pressedButton = myButtons.checkButtons();

  if (pressedButton == menuButton)
  {
    MainMenu();
  }
};

void SetTimeButtonHendler()
{
  Time newTime = GetTime();

  while(1)
  {
    DisplayTime(newTime);
    if (myTouch.dataAvailable())
    {
      pressedButton = myButtons.checkButtons();

      if (pressedButton == hourUpButton && newTime.hour < 24)
      {
          newTime.hour = newTime.hour + 1;
      }
      else if (pressedButton == hourDownButton && newTime.hour > 0)
      {
        newTime.hour = newTime.hour - 1;
      }
      else if (pressedButton == minuteUpButton && newTime.minute <59)
      {
        newTime.minute = newTime.minute + 1;
      }
      else if (pressedButton == minuteDownButton && newTime.minute > 0)
      {
        newTime.minute = newTime.minute - 1;
      }
      else if (pressedButton == saveButton)
      {
        clock.setDateTime(newTime.year, newTime.month, newTime.day, newTime.hour, newTime.minute, newTime.second);
        break;
      }
      else if (pressedButton == cancelButton)
      {
        break;
      }
    }
  }
}

void SetDateButtonHendler()
{
  Time newDate = GetTime();

while(1)
  {
    DisplayDate(newDate);
    if (myTouch.dataAvailable())
    {
      pressedButton = myButtons.checkButtons();

      if (pressedButton == dayUpButton && newDate.day < 31)
      {
          newDate.day = newDate.day + 1;
      }
      else if (pressedButton == dayDownButton && newDate.day > 0)
      {
        newDate.day = newDate.day - 1;
      }
      else if (pressedButton == monthUpButton && newDate.month < 12)
      {
          newDate.month = newDate.month + 1;
      }
      else if (pressedButton == monthDownButton && newDate.month > 0)
      {
        newDate.month = newDate.month - 1;
      }
      else if (pressedButton == yearUpButton)
      {
          newDate.year = newDate.year + 1;
      }
      else if (pressedButton == yearDownButton && newDate.year > 2018)
      {
        newDate.year = newDate.year - 1;
      }
      else if (pressedButton == saveButton)
      {
        clock.setDateTime(newDate.year, newDate.month, newDate.day, newDate.hour, newDate.minute, newDate.second);
        break;
      }
      else if (pressedButton == cancelButton)
      {
        break;
      }
    }
  }
}

void MainMenuButtonHendler()
{
  while(1)
  {
    if (myTouch.dataAvailable())
    {
      pressedButton = myButtons.checkButtons();

      if (pressedButton == closeMenuButton)
      {
      break;
      }
      else if (pressedButton == timeSettingsButton)
      {
        SetupTimeMenu();
        break;
      }
      else if (pressedButton == dateSettingsButton)
      {
        SetupDateMenu();
        break;
      }
    }
  }
}