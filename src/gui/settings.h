#ifndef SETTINGS_H__
#define SETTINGS_H__


#include "guiInterface.h"


class Settings : public GuiInterface {
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

  enum EncoderPurpose {
    menu,
    BtVibIntensity,
    BtDeviceSelect,
    PwmLowIntensity,
    PwmHighIntensity
  };
  void setEncoderPurpose(EncoderPurpose e);
  EncoderPurpose encoderMode = menu;
  int32_t tempEncoderVal = 0;

  void drawIndex(uint8_t index);
};

#endif
