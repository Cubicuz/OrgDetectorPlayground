#include "vibtest.h"
#include "guiStuff.h"
#include "../toy/toyManager.h"


void Vibtest::draw() {
  display->clearDisplay();

  // adc value
  display->setCursor(0, 0);
  display->println(adcValue);

  display->print("Str: ");
  display->println(intensity);

  display->display();
}

void Vibtest::init(Adafruit_SSD1306 *dp, i2cEncoderMiniLib *enc)
{
  display = dp;
  encoder = enc;
}

void Vibtest::setup()
{
  encoder->begin(i2cEncoderMiniLib::WRAP_DISABLE | i2cEncoderMiniLib::DIRE_LEFT | i2cEncoderMiniLib::IPUP_ENABLE | i2cEncoderMiniLib::RMOD_X1);
  encoder->writeMin(0);
  encoder->writeMax(maxIntensity);
  encoder->writeStep(1);
  encoder->writeCounter(intensity);

  display->clearDisplay();
  display->setCursor(0, 0);
  display->setTextSize(2);
  display->setTextColor(WHITE, BLACK);

  draw();
}

void Vibtest::setAdcValue(int16_t value)
{
  adcValue = value;
  draw();
}

void Vibtest::handleButtonPush()
{
  setIntensity(0);
}

void Vibtest::handleButtonLongPush()
{
  GuiStuff::setActiveGui(&GuiStuff::guiMenu);
  ToyManager::Instance->setIntensity(0);
}

void Vibtest::handleEncoderChange(int32_t position)
{
  setIntensity(position);
}

void Vibtest::setIntensity(uint8_t intensity)
{
  this->intensity = intensity;
  ToyManager::Instance->setIntensity(intensity);
  draw();
}
