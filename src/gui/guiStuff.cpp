#include "guiStuff.h"

GuiInterface* GuiStuff::activeGui = NULL;
Menu GuiStuff::guiMenu{};
Play GuiStuff::guiPlay{};
BluetoothSettings GuiStuff::guiBluetoothSettings{};

void GuiStuff::initializeGuis(Adafruit_SSD1306 *dp, i2cEncoderMiniLib *enc)
{
  guiMenu.init(dp, enc);
  guiPlay.init(dp, enc);
  guiBluetoothSettings.init(dp, enc);
  
  guiMenu.addMenuEntry(" play", &guiPlay);
  guiMenu.addMenuEntry(" bluetth", &guiBluetoothSettings);

  setActiveGui(&guiMenu);
}

void GuiStuff::setActiveGui(GuiInterface *gui)
{
  activeGui = NULL;
  gui->setup();
  activeGui = gui;
}
