#include "preferencesManager.h"

PreferencesManager PreferencesManager::instance{};

void PreferencesManager::begin()
{
  preferences.begin("my-app", false);

  bluetoothEnabledValue = preferences.getBool(bluetoothEnabledKey.c_str(), true);
}

bool PreferencesManager::bluetoothEnabled()
{
  return bluetoothEnabledValue;
}

void PreferencesManager::setBluetoothEnabled(bool value)
{
  bluetoothEnabledValue = value;
  preferences.putBool(bluetoothEnabledKey.c_str(), value);
}
