#ifndef MENU_H
#define MENU_H

#include "guiInterface.h"
#include <vector>

class Menu : public GuiInterface{
public:
  void draw() override;
  void init(Adafruit_SSD1306* dp, i2cEncoderMiniLib* enc) override;
  virtual void setup() override;
  // userinput
  void handleButtonPush() override;
  void handleEncoderChange(int32_t position) override;
  void addMenuEntry(String name, GuiInterface* gui);

private:
  // selected menu entry
  uint8_t selectedMenuEntryIndex{};
  // menu entries
  std::vector<String> menuEntries{}; 
  std::vector<GuiInterface*> guiEntries{};
};

#endif
