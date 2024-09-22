#ifndef PLAY_H
#define PLAY_H

#include "guiInterface.h"

class Play : public GuiInterface
{
public:
  void draw() override;
  void init(Adafruit_SSD1306 *dp, i2cEncoderMiniLib *enc) override;
  void setup() override;

  // userinput
  void handleButtonLongPush() override;
  void handleEncoderChange(int32_t position) override;

private:
  uint8_t maxIntensity = 50;
};

#endif
