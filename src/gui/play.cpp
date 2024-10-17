#include "play.h"
#include "guiStuff.h"
#include "../toy/toyManager.h"
#include "../sensor/detector.h"


void Play::draw() {
  display->clearDisplay();

  // adc value
  display->setCursor(0, 0);
  display->println(adcValue);

  display->print("tresh:");
  display->println(Detector::instance.getBoringToFunThresh());

  switch (Detector::instance.getState())
  {
  case Detector::State::BORING:
    display->println("boring");
    break;
  case Detector::State::COOLDOWN:
    display->println("cooldown");
    break;
  case Detector::State::FUN:
    display->println("fun");
    break;  
  default:
    break;
  }

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
  encoder->writeMin(500);
  encoder->writeMax(0xFFF);
  encoder->writeStep(1);
  encoder->writeCounter(maxIntensity);
  encoder->writeCounter(Detector::instance.getBoringToFunThresh());
  draw();
}

void Play::setAdcValue(int16_t value)
{
  adcValue = value;
  draw();
}

void Play::handleButtonPush()
{

}

void Play::handleButtonLongPush()
{
  GuiStuff::setActiveGui(&GuiStuff::guiMenu);
  ToyManager::Instance->setIntensity(0);
}

void Play::handleEncoderChange(int32_t position)
{
  Detector::instance.setBoringToFunThresh(position);
}

void Play::setIntensity(uint8_t intensity)
{
  ToyManager::Instance->setIntensity(intensity);
}
