#ifndef GUISTUFF_H
#define GUISTUFF_H

#include "menu.h"
#include "play.h"


class GuiStuff{
public:
  static  Menu guiMenu;
  static Play guiPlay;

  static GuiInterface *activeGui;
  
  static void setActiveGui(GuiInterface *gui);
  static void initializeGuis(Adafruit_SSD1306 *dp, i2cEncoderMiniLib *enc);
};

#endif
