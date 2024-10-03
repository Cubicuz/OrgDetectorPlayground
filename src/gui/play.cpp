#include "play.h"
#include "guiStuff.h"
#include "../toy/toyManager.h"

void Play::draw() {
  display->clearDisplay();

  // adc value
  display->setCursor(0, 0);
  display->print(adcValue);

  // botom bar
  display->drawRect(0, 57, 105, 7, WHITE);
  int16_t horizontalPosition = maxIntensity + 2;
  display->drawLine(horizontalPosition, 59, horizontalPosition, 61, WHITE);

  display->display();
}

void Play::init(Adafruit_SSD1306 *dp, i2cEncoderMiniLib *enc)
{
  display = dp;
  encoder = enc;
}

void Play::setup()
{
  encoder->begin(i2cEncoderMiniLib::WRAP_DISABLE | i2cEncoderMiniLib::DIRE_LEFT | i2cEncoderMiniLib::IPUP_ENABLE | i2cEncoderMiniLib::RMOD_X1);
  encoder->writeMin(0);
  encoder->writeMax(100);
  encoder->writeStep(1);
  encoder->writeCounter(maxIntensity);

  draw();
}

void Play::setAdcValue(uint16_t value)
{
  adcValue = value;
  draw();
}

void Play::handleButtonPush()
{
  encoder->writeCounter(0);
  setIntensity(0);
}

void Play::handleButtonLongPush()
{
  GuiStuff::setActiveGui(&GuiStuff::guiMenu);
}

void Play::handleEncoderChange(int32_t position)
{
  setIntensity(position);
}

void Play::setIntensity(uint8_t intensity)
{
  maxIntensity = intensity;
  draw();
  ToyManager::Instance->setIntensity(intensity);
}
