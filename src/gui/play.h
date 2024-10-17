#ifndef PLAY_H
#define PLAY_H

#include "guiInterface.h"

class Play : public GuiInterface
{
public:
  void draw() override;
  void init(Adafruit_SSD1306 *dp, i2cEncoderMiniLib *enc) override;
  void setup() override;

  // adc input
  void setAdcValue(int16_t value);

  // userinput
  void handleButtonPush() override;
  void handleButtonLongPush() override;
  void handleEncoderChange(int32_t position) override;

private:
  int8_t maxIntensity = 0;
  void setIntensity(uint8_t intensity);
  int16_t adcValue = 0;
};

#endif
