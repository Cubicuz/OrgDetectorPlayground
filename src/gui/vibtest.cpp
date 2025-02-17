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

void Vibtest::init(Adafruit_SSD1306 *dp, i2cEncoderMiniLib *encLeft, i2cEncoderMiniLib *encRight)
{
  display = dp;
  encoderLeft = encLeft;
  encoderRight = encRight;
}

void Vibtest::setup()
{
  encoderLeft->begin(i2cEncoderMiniLib::WRAP_DISABLE | i2cEncoderMiniLib::DIRE_LEFT | i2cEncoderMiniLib::IPUP_ENABLE | i2cEncoderMiniLib::RMOD_X1);
  encoderLeft->writeMin(0);
  encoderLeft->writeMax(maxIntensity);
  encoderLeft->writeStep(1);
  encoderLeft->writeCounter(intensity);

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

void Vibtest::handleButtonPush(i2cEncoderMiniLib *obj)
{
  if (obj == encoderLeft){
    setIntensity(0);
  }
}

void Vibtest::handleButtonLongPush(i2cEncoderMiniLib *obj)
{
  if (obj == encoderLeft){
    GuiStuff::setActiveGui(&GuiStuff::guiMenu);
    ToyManager::instance.setIntensityInt(0);
  }
}

void Vibtest::handleEncoderChange(i2cEncoderMiniLib *obj, int32_t position)
{
  if (obj == encoderLeft){
    setIntensity(position);
  }
}

void Vibtest::setIntensity(uint8_t intensity)
{
  this->intensity = intensity;
  ToyManager::instance.setIntensityInt(intensity);
  draw();
}
