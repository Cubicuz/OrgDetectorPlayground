#ifndef TOYMANAGER_H
#define TOYMANAGER_H

#include <cstdint>
#include "toyBle.h"
#include "toyPWM.h"

class ToyManager {
public:

  void init(); // call this once to create the objects
  void checkConnections(); // call this regularly to reconnect to lost devices
  void setIntensityInt(int16_t intensity);
  void setIntensity(Intensity intensity);
  static ToyManager instance;
private:
  ToyBLE toyBLE;
  ToyPWM toyPWM;
  ToyManager() = default;
};

#endif
