void MainMenu()
{
  myGLCD.clrScr();
  myButtons.deleteAllButtons();
  timeSettingsButton = myButtons.addButton(10, 15, 300, 30, "Ustawienia zegara");
  dateSettingsButton = myButtons.addButton(10, 50, 300, 30, "Ustawienia daty");
  smogSensorSettingsButton = myButtons.addButton(10, 85, 300, 30, "Ustawienia czujnika");
  closeMenuButton = myButtons.addButton(10, 120, 300, 30, "Zamknij");

  myButtons.drawButtons();

  MainMenuButtonHendler();
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

  SetTimeButtonHendler();
  MainMenu();
};

void SetupDateMenu()
{
  myGLCD.clrScr();
  myButtons.deleteAllButtons();

  dayUpButton = myButtons.addButton(10, 100, 80, 30, "Dzien +");
  dayDownButton = myButtons.addButton(10, 140, 80, 30, "Dzien -");
  monthUpButton = myButtons.addButton(120, 100, 80, 30, "Miesiac +");
  monthDownButton = myButtons.addButton(120, 140, 80, 30, "Miesiac -");
  yearUpButton = myButtons.addButton(230, 100, 80, 30, "Rok +");
  yearDownButton = myButtons.addButton(230, 140, 80, 30, "Rok -");
  saveButton = myButtons.addButton(220, 200, 80, 30, "Zapisz");
  cancelButton = myButtons.addButton(10, 200, 80, 30, "Zamknij");

  myButtons.drawButtons();

  SetDateButtonHendler();
  MainMenu();
};