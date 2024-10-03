#ifndef PREFERENCESMANAGER_H__
#define PREFERENCESMANAGER_H__

#include <Arduino.h>
#include <Preferences.h>

class PreferencesManager {
public:
  static PreferencesManager instance;

  void begin();


  bool bluetoothEnabled();
  void setBluetoothEnabled(bool value);

private:
  Preferences preferences;

  bool bluetoothEnabledValue = false;
  const String bluetoothEnabledKey = "btEnabled";

};

#endif
