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

  display->print("cd:");
  display->println(Detector::instance.getCooldownTimeSeconds()); 

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

void Play::init(Adafruit_SSD1306 *dp, i2cEncoderMiniLib *encLeft, i2cEncoderMiniLib *encRight)
{
  display = dp;
  encoderLeft = encLeft;
  encoderRight = encRight;
}

void Play::setup()
{
  encoderLeft->begin(i2cEncoderMiniLib::WRAP_DISABLE | i2cEncoderMiniLib::DIRE_LEFT | i2cEncoderMiniLib::IPUP_ENABLE | i2cEncoderMiniLib::RMOD_X1);
  encoderLeft->writeMin(500);
  encoderLeft->writeMax(0xFFF);
  encoderLeft->writeStep(1);
  encoderLeft->writeCounter(Detector::instance.getBoringToFunThresh());

  encoderRight->begin(i2cEncoderMiniLib::WRAP_DISABLE | i2cEncoderMiniLib::DIRE_LEFT | i2cEncoderMiniLib::IPUP_ENABLE | i2cEncoderMiniLib::RMOD_X1);
  encoderRight->writeMin(1);
  encoderRight->writeMax(50);
  encoderRight->writeStep(1);
  encoderRight->writeCounter(Detector::instance.getCooldownTimeSeconds());

  display->clearDisplay();
  display->setCursor(0, 0);
  display->setTextSize(2);
  display->setTextColor(WHITE, BLACK);

  draw();
}

void Play::setAdcValue(int16_t value)
{
  adcValue = value;
  draw();
}

void Play::handleButtonPush(i2cEncoderMiniLib *obj)
{
  if (obj == encoderLeft){
    Serial.print("<BUTTON>");
  }
}

void Play::handleButtonLongPush(i2cEncoderMiniLib *obj)
{
  if (obj == encoderLeft){
    GuiStuff::setActiveGui(&GuiStuff::guiMenu);
    ToyManager::Instance->setIntensity(Off);
  }
}

void Play::handleEncoderChange(i2cEncoderMiniLib *obj, int32_t position)
{
  if (obj == encoderLeft){
    Detector::instance.setBoringToFunThresh(position);
  } else if (obj == encoderRight){
    Detector::instance.setCooldownTimeSeconds(position);
  }
}

