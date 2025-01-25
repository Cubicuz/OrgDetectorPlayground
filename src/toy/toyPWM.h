#ifndef TOYPWM_H
#define TOYPWM_H

#include <Arduino.h>
#include "toyInterface.h"

class ToyPWM : public ToyInterface {
public:
  void init();
  void setIntensity(Intensity intensity) override;
  void setIntensityInt(int16_t intensity);
};

#endif
