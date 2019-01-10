void MainFrame()
{
  myGLCD.setBrightness(16);
  myGLCD.InitLCD();
  myGLCD.clrScr();
  myGLCD.setFont(SmallFont);
  myTouch.InitTouch();
  myTouch.setPrecision(PREC_MEDIUM);
  myButtons.setTextFont(SmallFont);
  myButtons.deleteAllButtons();
  menuButton = myButtons.addButton( 260, 40, 50, 50, "MENU");
  myButtons.drawButtons();
}

