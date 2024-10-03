#include "toyManager.h"
#include "toyBle.h"
#include "preferencesManager.h"

ToyManager* ToyManager::Instance;

void ToyManager::init()
{
  toyBLE.init();
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
}
