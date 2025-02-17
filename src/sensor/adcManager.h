#ifndef ADCMANAGER_H__
#define ADCMANAGER_H__

#include <Adafruit_ADS1X15.h>


class ADCManager {
public:
  static ADCManager instance;
  void init();
  bool updateValues();
  int16_t getAdcValue();
  uint32_t getMillisTimestamp();
private:
  Adafruit_ADS1015 ads;
  bool adsSuccessfullStarted = false;
  int16_t adcValue;
  uint32_t millisTimestamp;
  ADCManager() = default;
};


#endif
