#include <Arduino.h>
#include <Wire.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

#include <i2cEncoderMiniLib.h>

#include "gui/guiStuff.h"

#define I2C_0_SDA 16
#define I2C_0_SCL 17
#define I2C_0_CLOCK 100000
#define IntPin 19

Adafruit_SSD1306 display(128, 64, &Wire); // width, height
i2cEncoderMiniLib rotEncoder(0x20);

void draw();
void PrintHEX(uint8_t i);

void setupRotEncoder();
void on_encoder_Change(i2cEncoderMiniLib *obj);
void on_encoder_Increment(i2cEncoderMiniLib *obj);
void on_encoder_Decrement(i2cEncoderMiniLib *obj);
void on_encoder_Max(i2cEncoderMiniLib *obj);
void on_encoder_Min(i2cEncoderMiniLib *obj);
void on_encoder_ButtonPush(i2cEncoderMiniLib *obj);
void on_encoder_ButtonRelease(i2cEncoderMiniLib *obj);
void on_encoder_ButtonDoublePush(i2cEncoderMiniLib *obj);
void on_encoder_ButtonLongPush(i2cEncoderMiniLib *obj);

int32_t encoderPosition = 0;

// bluetooth
int scanTime = 5; // seconds
BLEScan* pBLEScan;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    if (advertisedDevice.getAddress().equals(BLEAddress("c4:4f:50:91:71:7e"))){
      Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());

    }
  }
};


void setup()
{
  // put your setup code here, to run once:

  Serial.begin(115200);
  pinMode(IntPin, INPUT);

  if (!Wire.begin(I2C_0_SDA, I2C_0_SCL, I2C_0_CLOCK))
  {
    // error
    Serial.write("error at Wire begin");
  }
  else
  {
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3c))
    {
      // error
      Serial.write("error at display begin");
    }
    else
    {
      Serial.write("Serial okay");
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(SSD1306_WHITE);
      display.print("Ready!!!");
      display.display();

      setupRotEncoder();

      GuiStuff::initializeGuis(&display, &rotEncoder);


      Serial.write("everything alright!\n");


      BLEDevice::init("");
      pBLEScan = BLEDevice::getScan(); // create new scan
      pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
      pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
      pBLEScan->setInterval(100);
      pBLEScan->setWindow(99); // less or equal setInterval value
    }
  }
}
// lovense target: LVS-Z44226, Address: c4:4f:50:91:71:7e, serviceUUID: 5a300001-0023-4bd4-bbd5-a6920e4c5653

void loop()
{
  // put your main code here, to run repeatedly:
  
  rotEncoder.updateStatus();
  
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);

  Serial.print("Devices found: ");
  Serial.println(foundDevices.getCount());
  Serial.println("Scan done!");

  BLEClient bleclient{};
  for (int i=0;i<foundDevices.getCount();i++){
    auto device = foundDevices.getDevice(i);
    if (device.getAddress().equals(BLEAddress("c4:4f:50:91:71:7e"))){
      if (bleclient.connect(&device)){
        Serial.println("connected");
        auto services = bleclient.getServices();
        auto serviceMap = *services;
        for (const auto& kv : serviceMap){
          kv.second->getCharacteristics();
          Serial.println(kv.second->toString().c_str());
        }
        BLEUUID serviceUUID1("00001800-0000-1000-8000-00805f9b34fb");
        BLEUUID charac1("00002a00-0000-1000-8000-00805f9b34fb"); // nothing here
        BLEUUID charac2("00002a01-0000-1000-8000-00805f9b34fb"); // nothing here
        BLEUUID charac3("00002a04-0000-1000-8000-00805f9b34fb"); // nothing here
        BLEUUID charac4("00002aa6-0000-1000-8000-00805f9b34fb"); // nothing here
        BLEUUID serviceUUID2("5a300001-0023-4bd4-bbd5-a6920e4c5653");
        BLEUUID charac5("5a300002-0023-4bd4-bbd5-a6920e4c5653"); // here seems to be what i am looking for
        BLEUUID charac6("5a300003-0023-4bd4-bbd5-a6920e4c5653"); // something with patterns here
        bleclient.getService(serviceUUID1)->setValue(charac4, "Vibrate:10;");

        Serial.println("char1");
        Serial.println( (bleclient.getService(serviceUUID1)->getValue(charac1)).c_str());
        Serial.println("char2");
        Serial.println( (bleclient.getService(serviceUUID1)->getValue(charac2)).c_str());
        Serial.println("char3");
        Serial.println( (bleclient.getService(serviceUUID1)->getValue(charac3)).c_str());
        Serial.println("char4");
        Serial.println( (bleclient.getService(serviceUUID1)->getValue(charac4)).c_str());
        Serial.println("char5");
        Serial.println( (bleclient.getService(serviceUUID2)->getValue(charac5)).c_str());
        Serial.println("char6");
        Serial.println( (bleclient.getService(serviceUUID2)->getValue(charac6)).c_str());


      } else {
        Serial.println("could not connect to hush");
      }
    }
  }
  pBLEScan->clearResults();
  
  delay(5000);
}

void draw()
{
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextColor(SSD1306_WHITE);
  display.printf("Position: %i", encoderPosition);

  if (encoderPosition > 0)
  {
    display.drawRect(64, 53, encoderPosition, 10, SSD1306_WHITE);
  }
  else if (encoderPosition < 0)
  {
    display.drawRect(64 + encoderPosition, 53, abs(encoderPosition), 10, SSD1306_WHITE);
  }
  display.display();
}

void setupRotEncoder()
{
  rotEncoder.reset();
  rotEncoder.begin(i2cEncoderMiniLib::WRAP_ENABLE | i2cEncoderMiniLib::DIRE_LEFT | i2cEncoderMiniLib::IPUP_ENABLE | i2cEncoderMiniLib::RMOD_X1);
  rotEncoder.writeCounter((int32_t)0);  /* Set the step to 1*/
  rotEncoder.writeDoublePushPeriod(50); /*Set a period for the double push of 500ms */
  rotEncoder.writeMax((int32_t)64);     /* Set the maximum threshold*/
  rotEncoder.writeMin((int32_t)-64);    /* Set the minimum threshold */
  rotEncoder.writeStep((int32_t)1);     /* Set the step to 1*/

  rotEncoder.onChange = on_encoder_Change;
  rotEncoder.onIncrement = on_encoder_Increment;
  rotEncoder.onDecrement = on_encoder_Decrement;
  rotEncoder.onMax = on_encoder_Max;
  rotEncoder.onMin = on_encoder_Min;
  rotEncoder.onButtonPush = on_encoder_ButtonPush;
  rotEncoder.onButtonRelease = on_encoder_ButtonRelease;
  rotEncoder.onButtonDoublePush = on_encoder_ButtonDoublePush;
  rotEncoder.onButtonLongPush = on_encoder_ButtonLongPush;

  /* Enable the I2C Encoder V2 interrupts according to the previus attached callback */
  rotEncoder.autoconfigInterrupt();
}
void on_encoder_Change(i2cEncoderMiniLib *obj)
{
  encoderPosition = rotEncoder.readCounterLong();
  if (GuiStuff::activeGui != NULL)
  {
    GuiStuff::activeGui->handleUserInput();
    GuiStuff::activeGui->handleEncoderChange(encoderPosition);
  }
  Serial.println("Encoder is moved!");
  Serial.println(encoderPosition);
}

void on_encoder_Decrement(i2cEncoderMiniLib *obj)
{
  if (GuiStuff::activeGui != NULL)
  {
    GuiStuff::activeGui->handleUserInput();
  }
}
void on_encoder_Max(i2cEncoderMiniLib *obj)
{
  if (GuiStuff::activeGui != NULL)
  {
    GuiStuff::activeGui->handleUserInput();
  }
}
void on_encoder_Min(i2cEncoderMiniLib *obj)
{
  if (GuiStuff::activeGui != NULL)
  {
    GuiStuff::activeGui->handleUserInput();
  }
}
void on_encoder_ButtonPush(i2cEncoderMiniLib *obj)
{
  if (GuiStuff::activeGui != NULL)
  {
    GuiStuff::activeGui->handleUserInput();
    GuiStuff::activeGui->handleButtonPush();
  }
}
void on_encoder_ButtonRelease(i2cEncoderMiniLib *obj)
{
  if (GuiStuff::activeGui != NULL)
  {
    GuiStuff::activeGui->handleUserInput();
  }
}
void on_encoder_ButtonDoublePush(i2cEncoderMiniLib *obj)
{
  if (GuiStuff::activeGui != NULL)
  {
    GuiStuff::activeGui->handleUserInput();
  }
}
void on_encoder_ButtonLongPush(i2cEncoderMiniLib *obj)
{
  if (GuiStuff::activeGui != NULL)
  {
    GuiStuff::activeGui->handleUserInput();
    GuiStuff::activeGui->handleButtonLongPush();
  }
}
void on_encoder_Increment(i2cEncoderMiniLib *obj)
{
  if (GuiStuff::activeGui != NULL)
  {
    GuiStuff::activeGui->handleUserInput();
  }
}

void PrintHEX(uint8_t i)
{
  Serial.print("0x");
  if (i < 16)
    Serial.print("0");
  Serial.print(i, HEX);
}
