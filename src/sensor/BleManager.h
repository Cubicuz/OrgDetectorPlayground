#ifndef BLEMANAGER_H__
#define BLEMANAGER_H__

#include <Arduino.h>
#include <BLEDevice.h>

class BleManager {
public:
  static BleManager instance;
  void updateDetectorState(uint16_t state);
  void init();
  void startAdvertising();
  void stopAdvertising();

private:
  const String bleServerName = "OrgDetector";

  BLEScan *pBLEScan{};
  BLEClient bleclient{};

  BLEServer *pBleServer{};
  BLEService *pBleService{};
  BLEAdvertising *pBleAdvertising{};
  boolean isAdvertising{false};
  BleManager() = default;
};

#endif