#ifndef VIBTEST_H
#define VIBTEST_H

#include "guiInterface.h"

class Vibtest : public GuiInterface
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
  void setIntensity(uint8_t intensity);
  int16_t adcValue = 0;
  int16_t intensity = 0;
  int16_t maxIntensity = 1000;
};

#endif