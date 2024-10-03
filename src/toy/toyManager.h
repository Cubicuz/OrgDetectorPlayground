#ifndef TOYMANAGER_H
#define TOYMANAGER_H

#include <cstdint>
#include "toyBle.h"

class ToyManager {
public:
  void init(); // call this once to create the objects
  void checkConnections(); // call this regularly to reconnect to lost devices
  void setIntensity(int16_t intensity);
  static ToyManager* Instance;
private:
  ToyBLE toyBLE;

};

#endif
