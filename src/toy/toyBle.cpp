#include <Arduino.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

#include "toyBle.h"

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    if (advertisedDevice.getAddress().equals(BLEAddress("c4:4f:50:91:71:7e"))){
      Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());

    }
  }
};

void ToyBLE::init()
{
  pBLEScan = BLEDevice::getScan(); // create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99); // less or equal setInterval value

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
    if (device.getAddress().equals(myDeviceAddress)){
      if (bleclient.connect(&device)){
        Serial.println("connected");
        auto services = bleclient.getServices();
        auto serviceMap = *services;
        for (const auto& kv : serviceMap){
          kv.second->getCharacteristics();
          Serial.println(kv.second->toString().c_str());
        }
      } else {
        Serial.println("could not connect to hush");
      }
      break;
    }
  }
  pBLEScan->clearResults();

}

void ToyBLE::setIntensity(int16_t intensity)
{
  if (bleclient.isConnected()){

    this->intensity = constrain(intensity, 0, 100);
    String value = String("Vibrate:") + String(this->intensity);
    bleclient.getService(serviceUUID2)->setValue(charac5, value.c_str());
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
