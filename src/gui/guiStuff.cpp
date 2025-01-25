#include "guiStuff.h"

GuiInterface* GuiStuff::activeGui = NULL;
Menu GuiStuff::guiMenu{};
Play GuiStuff::guiPlay{};
Settings GuiStuff::guiSettings{};
Vibtest GuiStuff::guiVibtest{};

void GuiStuff::initializeGuis(Adafruit_SSD1306 *dp, i2cEncoderMiniLib *encLeft, i2cEncoderMiniLib *encRight)
{
  guiMenu.init(dp, encLeft, encRight);
  guiPlay.init(dp, encLeft, encRight);
  guiSettings.init(dp, encLeft, encRight);
  guiVibtest.init(dp, encLeft, encRight);

  guiMenu.addMenuEntry(" play", &guiPlay);
  guiMenu.addMenuEntry(" setting", &guiSettings);
  guiMenu.addMenuEntry(" vibtest", &guiVibtest);

  setActiveGui(&guiMenu);
}

void GuiStuff::setActiveGui(GuiInterface *gui)
{
  activeGui = NULL;
  gui->setup();
  activeGui = gui;
}
