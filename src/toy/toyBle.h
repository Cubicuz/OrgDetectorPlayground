#ifndef TOYBLE_H
#define TOYBLE_H
#include <Arduino.h>

#include <BLEDevice.h>
#include <vector>
#include "toyInterface.h"

class ToyBLE : public ToyInterface {
public:
  void init();
  void connect();
  void setIntensityInt(int16_t intensity);
  void setIntensity(Intensity intensity) override;
  void end();

  struct SelectableDevice {
    String displayName;
    BLEAddress address;
    BLEUUID serviceUUID;
    BLEUUID characteristic;
  };
  static const std::vector<SelectableDevice> selectableDevices;
  uint16_t fixedIndex{UINT16_MAX};
private:
// lovense target: LVS-Z44226, Address: c4:4f:50:91:71:7e, serviceUUID: 5a300001-0023-4bd4-bbd5-a6920e4c5653

  BLEScan* pBLEScan;
  BLEClient bleclient{};
  

  int scanTime = 1; // seconds
  int16_t intensity = 0;
};

#endif
