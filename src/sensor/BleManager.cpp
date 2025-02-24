#include "BleManager.h"
#include "../preferencesManager.h"
#include <BLEDevice.h>

BleManager BleManager::instance{};

const BLEUUID service_uuid = BLEUUID((uint16_t)0x181A);
static BLECharacteristic arousalStateCharacteristics = BLECharacteristic(
    BLEUUID((uint16_t)0x2A6C),//"6e7cdf2b-adbc-4525-8c86-3fd04e3691cc"),
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
static BLEDescriptor arousalStateDescriptor = BLEDescriptor(BLEUUID((uint16_t)0x2902));

static bool deviceConnected = false;

class BleServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer) {
    deviceConnected = true;
    Serial.println("Device Connected");
  }
  void onDisconnect(BLEServer *pServer) {
    deviceConnected = false;
    Serial.println("Device Disconnected");
  }
};

void BleManager::updateDetectorState(uint16_t state) {
  if (deviceConnected) {
    arousalStateCharacteristics.setValue(state);
    arousalStateCharacteristics.notify();
  }
}

void BleManager::init() {
  BLEDevice::init(bleServerName.c_str());
  pBleServer = BLEDevice::createServer();
  pBleServer->setCallbacks(new BleServerCallbacks());

  pBleService = pBleServer->createService(service_uuid);

  pBleService->addCharacteristic(&arousalStateCharacteristics);
  arousalStateCharacteristics.addDescriptor(&arousalStateDescriptor);

  pBleService->start();
  pBleAdvertising = pBleServer->getAdvertising();

  updateDetectorState(0);
}

void BleManager::startAdvertising() {
  if (pBleAdvertising != nullptr 
    && PreferencesManager::instance.bluetoothServiceEnabled() 
    && !isAdvertising) {
    isAdvertising = true;
    pBleAdvertising->start();
    Serial.println("start advertising");
    Serial.println(BLEDevice::getAddress().toString().c_str());
  }
}

void BleManager::stopAdvertising() {
  if (pBleAdvertising != nullptr && isAdvertising) {
    isAdvertising = false;
    pBleAdvertising->stop();
    Serial.println("stop advertising");
  }
}
