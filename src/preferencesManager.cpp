#include "preferencesManager.h"

PreferencesManager PreferencesManager::instance{};

void PreferencesManager::begin()
{
  preferences.begin("my-app", false);

  bluetoothEnabledValue = preferences.getBool(bluetoothEnabledKey.c_str(), true);
  boringToFunThresholdValue = preferences.getShort(boringToFunThresholdKey.c_str(), 700);
  bluetoothIntensityValue = preferences.getShort(bluetoothEnabledKey.c_str(), 15);
  selectedBluetoothDeviceIndexValue = preferences.getUShort(selectedBluetoothDeviceIndexKey.c_str(), 0);

  pwmToyEnabledValue = preferences.getBool(pwmToyEnabledKey.c_str(), true);
  pwmToyLowIntensityValue = preferences.getUShort(pwmToyLowIntensityKey.c_str(), 16);
  pwmToyHighIntensityValue = preferences.getUShort(pwmToyHighIntensityKey.c_str(), 255);

  adcReference1BarValue = preferences.getShort(adcReference1BarKey.c_str(), 1821);
}

bool PreferencesManager::bluetoothEnabled()
{
  return bluetoothEnabledValue;
}

void PreferencesManager::setBluetoothEnabled(bool value)
{
  if (value != bluetoothEnabledValue){
    bluetoothEnabledValue = value;
    preferences.putBool(bluetoothEnabledKey.c_str(), value);
  }
}

int16_t PreferencesManager::boringToFunThreshold()
{
  return boringToFunThresholdValue;
}

void PreferencesManager::setBoringToFunThreshold(int16_t value)
{
  if (value != boringToFunThresholdValue){
    preferences.putShort(boringToFunThresholdKey.c_str(), value);
    boringToFunThresholdValue = value;
  }
}

int16_t PreferencesManager::bluetoothIntensity()
{
  return bluetoothIntensityValue;
}

void PreferencesManager::setBloetoothIntensity(int16_t value)
{
  if (value != bluetoothIntensityValue){
    preferences.putShort(bluetoothIntensityKey.c_str(), value);
    bluetoothIntensityValue = value;
  }
}

bool PreferencesManager::pwmToyEnabled()
{
  return pwmToyEnabledValue;
}

void PreferencesManager::setPwmToyEnabled(bool value)
{
    if (value != pwmToyEnabledValue){
    pwmToyEnabledValue = value;
    preferences.putBool(pwmToyEnabledKey.c_str(), value);
  }
}

uint16_t PreferencesManager::pwmToyLowIntensity()
{
  return pwmToyLowIntensityValue;
}

void PreferencesManager::setPwmToyLowIntensity(uint16_t value)
{
  if (value != pwmToyLowIntensityValue){
    preferences.putUShort(pwmToyLowIntensityKey.c_str(), value);
    pwmToyLowIntensityValue = value;
  }
}

uint16_t PreferencesManager::pwmToyHighIntensity()
{
  return pwmToyHighIntensityValue;
}

void PreferencesManager::setPwmToyHighIntensity(uint16_t value)
{
  if (value != pwmToyHighIntensityValue){
    preferences.putUShort(pwmToyHighIntensityKey.c_str(), value);
    pwmToyHighIntensityValue = value;
  }
}

uint16_t PreferencesManager::selectedBluetoothDeviceIndex()
{
  return selectedBluetoothDeviceIndexValue;
}

void PreferencesManager::setSelectedBluetoothDeviceIndex(uint16_t value)
{
  if (value != selectedBluetoothDeviceIndexValue){
    preferences.putUShort(selectedBluetoothDeviceIndexKey.c_str(), value);
    selectedBluetoothDeviceIndexValue = value;
  }
}

int16_t PreferencesManager::adcReference1Bar()
{
    return adcReference1BarValue;
}

void PreferencesManager::setAdcReference1Bar(int16_t value) {
    if (value != adcReference1BarValue){
        preferences.putShort(adcReference1BarKey.c_str(), value);
        adcReference1BarValue = value;
    }
}
