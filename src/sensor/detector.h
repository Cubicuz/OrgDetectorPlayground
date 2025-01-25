#ifndef DETECTOR_H__
#define DETECTOR_H__

#include <Arduino.h>
#include <vector>



class Detector {
public:
  void putValue(int16_t input);

  enum State {
    BORING,
    FUN,
    COOLDOWN
  };
  State getState() { return state; };

  void setBoringToFunThresh(int16_t value);
  int16_t getBoringToFunThresh();
  void setCooldownTimeSeconds(uint8_t value);
  uint8_t getCooldownTimeSeconds();

  static Detector instance;
private:

  int16_t longAverage{};
  const int16_t longAverageDuration = 100;
  int32_t sumOfCurrentValues{};
  int16_t lowestValue{};
  int16_t noiseIntensity{};
  int16_t boringToFunThresh = 1850;
  int16_t funToBoringThresh = 1830;

  int16_t shortAverage{};
  const int16_t shortAverageDuration = 5;
  int32_t sumOfShortAverage{};
  
  State state = BORING;

  std::vector<int16_t> values;
  int16_t lastInsertedIndex{};
  int16_t lastInserted(int16_t offset = 0);

  uint32_t integrationCounter;
  const uint32_t integrationMax = 100000;

  unsigned long cooldownBeginTimestamp;
  unsigned long cooldownTimeInMillis = 5000;

  void updateState();

  Detector();
};

#endif
