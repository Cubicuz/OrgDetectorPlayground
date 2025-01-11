#include "guiStuff.h"

GuiInterface* GuiStuff::activeGui = NULL;
Menu GuiStuff::guiMenu{};
Play GuiStuff::guiPlay{};
Settings GuiStuff::guiSettings{};
Vibtest GuiStuff::guiVibtest{};

void GuiStuff::initializeGuis(Adafruit_SSD1306 *dp, i2cEncoderMiniLib *enc)
{
  guiMenu.init(dp, enc);
  guiPlay.init(dp, enc);
  guiSettings.init(dp, enc);
  guiVibtest.init(dp, enc);

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
