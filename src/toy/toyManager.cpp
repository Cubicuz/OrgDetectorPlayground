#include "toyManager.h"
#include "toyBle.h"
#include "preferencesManager.h"

ToyManager* ToyManager::Instance;

void ToyManager::init()
{
  toyBLE.init();
  toyPWM.init();
}

void ToyManager::checkConnections()
{
  if (PreferencesManager::instance.bluetoothEnabled()){
    toyBLE.connect();
  } else {
    toyBLE.end();
  }
}

void ToyManager::setIntensity(int16_t intensity)
{
  toyBLE.setIntensity(intensity);
  toyPWM.setIntensity(intensity);
}

void ToyManager::setIntensity(Intensity intensity)
{
  toyBLE.setIntensity(intensity);
  toyPWM.setIntensity(intensity);
}
