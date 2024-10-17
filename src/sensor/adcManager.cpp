#include "adcManager.h"

void ADCManager::init()
{
  adsSuccessfullStarted = ads.begin();
  if (adsSuccessfullStarted){
    //Serial.write("ads initialized!\n");
    ads.setGain(GAIN_FOUR);
  } else {
    Serial.write("error at ads begin\n");
  }
}

bool ADCManager::updateValues()
{
  if (adsSuccessfullStarted){
    int16_t newValue = ads.readADC_SingleEnded(0);
    millisTimestamp = millis();
    if (newValue != adcValue){
      adcValue = newValue;
      Serial.print("K");
      Serial.print(adcValue);
      return true;
    }
  }
  return false;
}

int16_t ADCManager::getAdcValue()
{
  return adcValue;
}

uint32_t ADCManager::getMillisTimestamp()
{
  return millisTimestamp;
}
