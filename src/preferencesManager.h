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

  int16_t boringToFunThreshold();
  void setBoringToFunThreshold(int16_t value);

  int16_t bluetoothIntensity();
  void setBloetoothIntensity(int16_t value);

  bool pwmToyEnabled();
  void setPwmToyEnabled(bool value);

  uint16_t pwmToyLowIntensity();
  void setPwmToyLowIntensity(uint16_t value);

  uint16_t pwmToyHighIntensity();
  void setPwmToyHighIntensity(uint16_t value);

  uint16_t selectedBluetoothDeviceIndex();
  void setSelectedBluetoothDeviceIndex(uint16_t value);
private:
  Preferences preferences;

  bool bluetoothEnabledValue = false;
  const String bluetoothEnabledKey = "btEnabled";

  int16_t boringToFunThresholdValue = 0;
  const String boringToFunThresholdKey = "btfthrsh";

  int16_t bluetoothIntensityValue = 0;
  const String bluetoothIntensityKey = "btintens";

  bool pwmToyEnabledValue = false;
  const String pwmToyEnabledKey = "pwmTEnabled";

  uint16_t pwmToyLowIntensityValue = 0;
  const String pwmToyLowIntensityKey = "pwmTLowInt";

  uint16_t pwmToyHighIntensityValue = 0;
  const String pwmToyHighIntensityKey = "pwmTHighInt";

  uint16_t selectedBluetoothDeviceIndexValue = 0;
  const String selectedBluetoothDeviceIndexKey = "btSelDevID";
};

#endif
