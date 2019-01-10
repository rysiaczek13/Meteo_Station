void CheckClickedButton()
{
  pressedButton = myButtons.checkButtons();

  if (pressedButton == menuButton)
  {
    MainMenu();
  }
};

void MainMenu()
{
  myGLCD.clrScr();
  myButtons.deleteAllButtons();
  timeSettingsButton = myButtons.addButton(10, 15, 300, 30, "Ustawienia zegara");
  dateSettingsButton = myButtons.addButton(10, 50, 300, 30, "Ustawienia daty");
  smogSensorSettingsButton = myButtons.addButton(10, 85, 300, 30, "Ustawienia czujnika");
  closeMenuButton = myButtons.addButton(10, 120, 300, 30, "Zamknij");

  myButtons.drawButtons();

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
    }
  }
  MainFrame();
};

void SetupTimeMenu()
{
  myGLCD.clrScr();
  myButtons.deleteAllButtons();

  hourUpButton = myButtons.addButton(10, 100, 80, 30, "Godzina +");
  hourDownButton = myButtons.addButton(10, 140, 80, 30, "Godzina -");
  minuteUpButton = myButtons.addButton(120, 100, 80, 30, "Minuta +");
  minuteDownButton = myButtons.addButton(120, 140, 80, 30, "Minuta -");
  saveButton = myButtons.addButton(220, 200, 80, 30, "Zapisz");
  cancelButton = myButtons.addButton(10, 200, 80, 30, "Zamknij");

  myButtons.drawButtons();

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
  MainMenu();
};