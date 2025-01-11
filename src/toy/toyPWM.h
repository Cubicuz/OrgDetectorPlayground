#ifndef TOYPWM_H
#define TOYPWM_H

#include <Arduino.h>
#include "toyInterface.h"

class ToyPWM : public ToyInterface {
public:
  void init();
  void setIntensity(int16_t intensity);
  void setIntensity(Intensity intensity) override;

};

#endif
