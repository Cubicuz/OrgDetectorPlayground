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
  display->print(Detector::instance.getCooldownTimeSeconds()); 

  switch (Detector::instance.getState())
  {
  case Detector::State::BORING:
    display->println(" low");
    break;
  case Detector::State::COOLDOWN:
    display->println(" chil");
    break;
  case Detector::State::FUN:
    display->println(" fun");
    break;  
  default:
    break;
  }
  display->print("il:");
  display->println(Detector::instance.getIntegrationLimit());


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

  setupRightEncoder();

  display->clearDisplay();
  display->setCursor(0, 0);
  display->setTextSize(2);
  display->setTextColor(WHITE, BLACK);

  draw();
}
void Play::setupRightEncoder()
{
  encoderRight->begin(i2cEncoderMiniLib::WRAP_DISABLE | i2cEncoderMiniLib::DIRE_LEFT | i2cEncoderMiniLib::IPUP_ENABLE | i2cEncoderMiniLib::RMOD_X1);
  if (rightEncoderState == Cooldown) {
    encoderRight->writeMin(1);
    encoderRight->writeMax(50);
    encoderRight->writeStep(1);
    encoderRight->writeCounter(Detector::instance.getCooldownTimeSeconds());
  } else if (rightEncoderState == Integral){
    encoderRight->writeMin(1);
    encoderRight->writeMax(250);
    encoderRight->writeStep(1);
    encoderRight->writeCounter(Detector::instance.getIntegrationLimit());
  }
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
  } else if (obj == encoderRight){
    if (rightEncoderState == Cooldown){
      rightEncoderState = Integral;
    } else if (rightEncoderState == Integral){
      rightEncoderState = Cooldown;
    }
    setupRightEncoder();
  }
}

void Play::handleButtonLongPush(i2cEncoderMiniLib *obj)
{
  if (obj == encoderLeft){
    GuiStuff::setActiveGui(&GuiStuff::guiMenu);
    ToyManager::instance.setIntensity(Off);
  }
}

void Play::handleEncoderChange(i2cEncoderMiniLib *obj, int32_t position)
{
  if (obj == encoderLeft){
    Detector::instance.setBoringToFunThresh(position);
  } else if (obj == encoderRight){
    if (rightEncoderState == Cooldown){
      Detector::instance.setCooldownTimeSeconds(position);
    } else if (rightEncoderState == Integral) {
      Detector::instance.setIntegrationLimit(position);
    }
  }
}

