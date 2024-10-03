#ifndef TOYBLE_H
#define TOYBLE_H

#include <BLEDevice.h>

#include "toyInterface.h"

class ToyBLE : public ToyInterface {
public:
  void init();
  void connect();
  void setIntensity(int16_t intensity);
  void end();

private:
// lovense target: LVS-Z44226, Address: c4:4f:50:91:71:7e, serviceUUID: 5a300001-0023-4bd4-bbd5-a6920e4c5653

  BLEUUID serviceUUID1{"00001800-0000-1000-8000-00805f9b34fb"};
  BLEUUID charac1{"00002a00-0000-1000-8000-00805f9b34fb"}; // nothing here
  BLEUUID charac2{"00002a01-0000-1000-8000-00805f9b34fb"}; // nothing here
  BLEUUID charac3{"00002a04-0000-1000-8000-00805f9b34fb"}; // nothing here
  BLEUUID charac4{"00002aa6-0000-1000-8000-00805f9b34fb"}; // nothing here
  BLEUUID serviceUUID2{"5a300001-0023-4bd4-bbd5-a6920e4c5653"};
  BLEUUID charac5{"5a300002-0023-4bd4-bbd5-a6920e4c5653"}; // here seems to be what i am looking for
  BLEUUID charac6{"5a300003-0023-4bd4-bbd5-a6920e4c5653"}; // something with patterns here
  
  BLEAddress myDeviceAddress{"c4:4f:50:91:71:7e"}; // my device

  BLEScan* pBLEScan;
  BLEClient bleclient{};
  

  int scanTime = 5; // seconds
  int16_t intensity = 0;
};

#endif
