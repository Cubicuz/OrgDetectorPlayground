#ifndef GUISTUFF_H
#define GUISTUFF_H

#include "menu.h"
#include "play.h"
#include "settings.h"
#include "vibtest.h"

class GuiStuff{
public:
  static  Menu guiMenu;
  static Play guiPlay;
  static Settings guiSettings;
  static Vibtest guiVibtest;

  static GuiInterface *activeGui;
  
  static void setActiveGui(GuiInterface *gui);
  static void initializeGuis(Adafruit_SSD1306 *dp, i2cEncoderMiniLib *encLeft, i2cEncoderMiniLib *encRight);
};

#endif
