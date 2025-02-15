#include <Arduino.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

#include "toyBle.h"
#include "preferencesManager.h"

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    if (advertisedDevice.getAddress().equals(BLEAddress("c4:4f:50:91:71:7e"))){
      Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());

    }
  }
};

const std::vector<ToyBLE::SelectableDevice> ToyBLE::selectableDevices{
    {"Hush Michael", BLEAddress("c4:4f:50:91:71:7e"), BLEUUID("5a300001-0023-4bd4-bbd5-a6920e4c5653"), BLEUUID("5a300002-0023-4bd4-bbd5-a6920e4c5653")},
    {"Max nd", BLEAddress("dc:98:65:69:16:c4"), BLEUUID("42300001-0023-4bd4-bbd5-a6920e4c5653"), BLEUUID("42300002-0023-4bd4-bbd5-a6920e4c5653")},
    {"Domi2 nd", BLEAddress("f8:44:77:1d:27:e3"), BLEUUID("57300001-0023-4bd4-bbd5-a6920e4c5653"), BLEUUID("57300002-0023-4bd4-bbd5-a6920e4c5653")},
    {"Domi michi", BLEAddress("6c:5c:b1:46:60:ab"), BLEUUID("57300001-0023-4bd4-bbd5-a6920e4c5653"), BLEUUID("57300002-0023-4bd4-bbd5-a6920e4c5653")},

  };

void ToyBLE::init()
{
  pBLEScan = BLEDevice::getScan(); // create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99); // less or equal setInterval value

}
static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
    
    // al this is useless for me if I cannot broadcast it using my utils.
    // can I make notifyCallback part of an object?
    Serial.print("Notify callback for characteristic ");
    Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
    Serial.print(" of data length ");
    Serial.println(length);
    Serial.print("data: ");
    Serial.printf("%.*s\n", length, (char*)pData);
}

void ToyBLE::connect()
{
  if (bleclient.isConnected()){
    return;
  }
  if (!BLEDevice::getInitialized()){
    BLEDevice::init("");
  }

  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);

  Serial.print("Devices found: ");
  Serial.println(foundDevices.getCount());
  Serial.println("Scan done!");

  
  for (int i=0;i<foundDevices.getCount();i++){
    auto device = foundDevices.getDevice(i);
    Serial.println(device.toString().c_str());
    auto selectedIndex = PreferencesManager::instance.selectedBluetoothDeviceIndex();
    if (device.getAddress().equals(selectableDevices[selectedIndex].address)){
      if (bleclient.connect(&device)){
        Serial.println("connected");
        auto services = bleclient.getServices();
        auto serviceMap = *services;
        for (const auto& kv : serviceMap){
          kv.second->getCharacteristics();
          Serial.println(kv.second->toString().c_str());

        }
        auto tests5 = BLEUUID("57300001-0023-4bd4-bbd5-a6920e4c5653");
        auto tests5c1 = BLEUUID("57300002-0023-4bd4-bbd5-a6920e4c5653"); // w
        auto tests5c2 = BLEUUID("57300003-0023-4bd4-bbd5-a6920e4c5653"); // n

        auto caracteristicsMap = *(bleclient.getService(selectableDevices[selectedIndex].serviceUUID)->getCharacteristics());
        for (const auto& kv : caracteristicsMap){
          if (kv.second->canNotify()){
            kv.second->registerForNotify(&notifyCallback);
            Serial.println("callback set");
          }          
        }
        bleclient.getService(selectableDevices[selectedIndex].serviceUUID)->setValue(selectableDevices[selectedIndex].characteristic, "Battery;");

      } else {
        //Serial.println("could not connect to hush");
      }
      break;
    }
  }
  pBLEScan->clearResults();

}

void ToyBLE::setIntensityInt(int16_t intensity)
{
  if (bleclient.isConnected()){
    int16_t newIntensity = constrain(intensity, 0, 100);
    if (this->intensity != newIntensity){
      this->intensity = newIntensity;
      String value = String("Vibrate:") + String(this->intensity);
      uint16_t selectedIndex = PreferencesManager::instance.selectedBluetoothDeviceIndex();
      bleclient.getService(selectableDevices[selectedIndex].serviceUUID)->setValue(selectableDevices[selectedIndex].characteristic, value.c_str());
    }
  }
}
void ToyBLE::setIntensity(Intensity intensity)
{
  switch (intensity)
  {
  case Off:
    setIntensityInt(0);
    break;
  case Low:
    setIntensityInt(5);
  case High:
    setIntensityInt(25);
  default:
    break;
  }
}
void ToyBLE::end()
{
  if (bleclient.isConnected()){
    bleclient.disconnect();
  }
  if (BLEDevice::getInitialized()){
    BLEDevice::deinit();
  }
}
