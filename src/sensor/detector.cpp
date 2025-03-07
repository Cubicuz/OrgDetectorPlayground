#include "detector.h"
#include "../preferencesManager.h"

Detector Detector::instance{};

Detector::Detector(){
  values.reserve(longAverageDuration);
  for (int i=0;i<longAverageDuration;i++){
    values.push_back(0);
  }
}

void Detector::putValue(int16_t input){
    sumOfCurrentValues -= lastInserted(1);
    sumOfCurrentValues += input;
    longAverage = sumOfCurrentValues / longAverageDuration;
    
    sumOfShortAverage -= lastInserted(-shortAverageDuration);
    sumOfShortAverage += input;
    shortAverage = sumOfShortAverage / (shortAverageDuration+1);
    
    lastInsertedIndex = (lastInsertedIndex+1)%longAverageDuration;
    values[lastInsertedIndex] = input;
    updateState();
}

void Detector::setBoringToFunThresh(int16_t value)
{
  value = constrain(value, 21, 0xFFF);
  if (value != boringToFunThresh){
    boringToFunThresh = value;
    funToBoringThresh = value - 20;
    if (funToBoringThresh <= PreferencesManager::instance.adcReference1Bar()){
      funToBoringThresh = PreferencesManager::instance.adcReference1Bar()+1;
    }
  }
}

int16_t Detector::getBoringToFunThresh()
{
  return boringToFunThresh;
}

void Detector::setCooldownTimeSeconds(uint8_t value)
{
  cooldownTimeInMillis = ((unsigned long)value) * 1000;
}

uint8_t Detector::getCooldownTimeSeconds()
{
  return (uint8_t) (cooldownTimeInMillis/1000);
}

void Detector::setIntegrationLimit(uint32_t value)
{
  integrationMax = value * 1000;
}

uint32_t Detector::getIntegrationLimit()
{
  return (uint32_t) (integrationMax/1000);
}

int16_t Detector::lastInserted(int16_t offset)
{
  return values[(lastInsertedIndex + longAverageDuration + offset)%longAverageDuration];
}

void Detector::updateState()
{
  State nextState = state;
  switch (state)
  {
  case BORING:
    if (shortAverage > boringToFunThresh){
      nextState = FUN;
      integrationCounter = 0;
    }
    if (shortAverage < (PreferencesManager::instance.adcReference1Bar()-5)){
      // vacuum problem
      nextState = COOLDOWN;
      cooldownBeginTimestamp = millis();
    }
    break;
  case FUN:
    if (shortAverage < (PreferencesManager::instance.adcReference1Bar()-5)){
      // vacuum problem
      nextState = COOLDOWN;
      cooldownBeginTimestamp = millis();
    } else if (shortAverage < funToBoringThresh){
      nextState = BORING;
    } else {
      integrationCounter += lastInserted();
      if (integrationCounter > integrationMax){
        nextState = COOLDOWN;
        cooldownBeginTimestamp = millis();
      }
    }
    break;
  case COOLDOWN:
    if ( (shortAverage < funToBoringThresh) 
      && (cooldownBeginTimestamp + cooldownTimeInMillis < millis()))
    {
      nextState = BORING;
    }
    break;
  default:
    break;
  }

  if (nextState != state){
    state = nextState;
    //BleManager::instance.updateDetectorState((uint16_t)state);
  }
}
