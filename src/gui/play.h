#ifndef PLAY_H
#define PLAY_H

#include "guiInterface.h"

class Play : public GuiInterface
{
public:
  void draw() override;
  void init(Adafruit_SSD1306 *dp, i2cEncoderMiniLib *encLeft, i2cEncoderMiniLib *encRight) override;
  void setup() override;

  // adc input
  void setAdcValue(int16_t value);

  // userinput
  void handleButtonPush(i2cEncoderMiniLib *obj) override;
  void handleButtonLongPush(i2cEncoderMiniLib *obj) override;
  void handleEncoderChange(i2cEncoderMiniLib *obj, int32_t position) override;

private:
  int8_t maxIntensity = 0;
  int16_t adcValue = 0;
  enum RightEncoderState{
    Cooldown,
    Integral
  };
  RightEncoderState rightEncoderState = Cooldown;
  void setupRightEncoder();
};

#endif
