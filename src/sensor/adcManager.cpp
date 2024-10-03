#include "adcManager.h"

void ADCManager::init()
{
  adsSuccessfullStarted = ads.begin();
  if (adsSuccessfullStarted){
    //Serial.write("ads initialized!\n");
  } else {
    Serial.write("error at ads begin\n");
  }
}

bool ADCManager::updateValues()
{
  if (adsSuccessfullStarted){
    int16_t newValue = ads.readADC_SingleEnded(0);
    if (newValue != adcValue){
      adcValue = newValue;
      return true;
    }
  }
  return false;
}

int16_t ADCManager::getAdcValue()
{
  return adcValue;
}
