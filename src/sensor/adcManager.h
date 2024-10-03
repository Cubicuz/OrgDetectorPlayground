#ifndef ADCMANAGER_H__
#define ADCMANAGER_H__

#include <Adafruit_ADS1X15.h>


class ADCManager {
public:
  void init();
  bool updateValues();
  int16_t getAdcValue();
private:
  Adafruit_ADS1015 ads;
  bool adsSuccessfullStarted = false;
  int16_t adcValue;

};


#endif
