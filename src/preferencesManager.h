#ifndef PREFERENCESMANAGER_H__
#define PREFERENCESMANAGER_H__

#include <Arduino.h>
#include <Preferences.h>

class PreferencesManager {
public:
  static PreferencesManager instance;

  void begin();


  bool bluetoothToyEnabled();
  void setBluetoothToyEnabled(bool value);

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

  int16_t adcReference1Bar();
  void setAdcReference1Bar(int16_t value);
private:
  Preferences preferences;

  bool bluetoothToyEnabledValue = false;
  const String bluetoothToyEnabledKey = "btEnabled";

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

  int16_t adcReference1BarValue = 0;
  const String adcReference1BarKey = "adcRef1Bar";
};

#endif
