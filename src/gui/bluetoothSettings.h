#ifndef BLUETOOTHSETTINGS_H__
#define BLUETOOTHSETTINGS_H__

#include "guiInterface.h"


class BluetoothSettings : public GuiInterface {
public:
  void draw() override;
  void init(Adafruit_SSD1306* dp, i2cEncoderMiniLib* enc) override;
  virtual void setup() override;
  // userinput
  void handleButtonPush() override;
  void handleButtonLongPush() override;
  void handleEncoderChange(int32_t position) override;

  
private:
  // selected menu entry
  int8_t selectedIndex{};


  void drawIndex(uint8_t index);
};

#endif
