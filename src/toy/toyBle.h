#ifndef TOYBLE_H
#define TOYBLE_H

#include <BLEDevice.h>

#include "toyInterface.h"

class ToyBLE : public ToyInterface {
public:
  void connect();
  void setIntensity(int16_t intensity);

private:
  BLEUUID serviceUUID1("00001800-0000-1000-8000-00805f9b34fb");
  BLEUUID charac1("00002a00-0000-1000-8000-00805f9b34fb"); // nothing here
  BLEUUID charac2("00002a01-0000-1000-8000-00805f9b34fb"); // nothing here
  BLEUUID charac3("00002a04-0000-1000-8000-00805f9b34fb"); // nothing here
  BLEUUID charac4("00002aa6-0000-1000-8000-00805f9b34fb"); // nothing here
  BLEUUID serviceUUID2("5a300001-0023-4bd4-bbd5-a6920e4c5653");
  BLEUUID charac5("5a300002-0023-4bd4-bbd5-a6920e4c5653"); // here seems to be what i am looking for
  BLEUUID charac6("5a300003-0023-4bd4-bbd5-a6920e4c5653"); // something with patterns here
};

#endif
