#include "detector.h"

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
    if (funToBoringThresh <= OneBarReference){
      funToBoringThresh = OneBarReference+1;
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
  switch (state)
  {
  case BORING:
    if (shortAverage > boringToFunThresh){
      state = FUN;
      integrationCounter = 0;
    }
    if (shortAverage < OneBarReference){
      // vacuum problem
      state = COOLDOWN;
      cooldownBeginTimestamp = millis();
    }
    break;
  case FUN:
    if (shortAverage < OneBarReference){
      // vacuum problem
      state = COOLDOWN;
      cooldownBeginTimestamp = millis();
    } else if (shortAverage < funToBoringThresh){
      state = BORING;
    } else {
      integrationCounter += lastInserted();
      if (integrationCounter > integrationMax){
        state = COOLDOWN;
        cooldownBeginTimestamp = millis();
      }
    }
    break;
  case COOLDOWN:
    if ( (shortAverage < funToBoringThresh) 
      && (cooldownBeginTimestamp + cooldownTimeInMillis < millis()))
    {
      state = BORING;
    }
    break;
  default:
    break;
  }
}
