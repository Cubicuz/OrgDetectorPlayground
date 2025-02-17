#include <Arduino.h>
#include <Wire.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <i2cEncoderMiniLib.h>

#include "gui/guiStuff.h"
#include "toy/toyManager.h"
#include "sensor/adcManager.h"
#include "preferencesManager.h"
#include "sensor/detector.h"

#define I2C_0_SDA 16
#define I2C_0_SCL 17
#define I2C_0_CLOCK 100000
#define IntPin 19

// I2C Buss clients:
// 0x20 encoder
// 0x21 encoder2
// 0x48 ADC
// 0x3C Display


Adafruit_SSD1306 display(128, 64, &Wire); // width, height
i2cEncoderMiniLib rotEncoder(0x20);
i2cEncoderMiniLib rotEncoder2(0x21);

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
      GuiStuff::initializeGuis(&display, &rotEncoder, &rotEncoder2);

      ADCManager::instance.init();
      ToyManager::instance.init();
      PreferencesManager::instance.begin();
    }
  }
}
unsigned long nextBluetoothRefreshTimestamp = 0;
void loop()
{
  unsigned long currentTimeStamp = millis();
  // put your main code here, to run repeatedly:
  
  rotEncoder.updateStatus();
  rotEncoder2.updateStatus();
  
  if ((GuiStuff::activeGui == &GuiStuff::guiPlay)){
    ADCManager::instance.updateValues();
    Detector::instance.putValue(ADCManager::instance.getAdcValue());
    switch (Detector::instance.getState())
    {
    case Detector::State::BORING:
      ToyManager::instance.setIntensity(High);
      break;
    case Detector::State::FUN:
      ToyManager::instance.setIntensity(Low);
      break;
    case Detector::State::COOLDOWN:
      ToyManager::instance.setIntensity(Off);
      break;
    default:
      break;
    }
    GuiStuff::guiPlay.setAdcValue(ADCManager::instance.getAdcValue());
  } else if (GuiStuff::activeGui == &GuiStuff::guiVibtest){
    ADCManager::instance.updateValues();
    GuiStuff::guiVibtest.setAdcValue(ADCManager::instance.getAdcValue());
  }
  if (currentTimeStamp > nextBluetoothRefreshTimestamp) { // check bluetooth once per 5 seconds
    ToyManager::instance.checkConnections();
    nextBluetoothRefreshTimestamp = currentTimeStamp + 5000; // bluetooth every 5 ms
  }
  
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
  rotEncoder.onButtonPush = on_encoder_ButtonPush;
  rotEncoder.onButtonRelease = on_encoder_ButtonRelease;
  rotEncoder.onButtonDoublePush = on_encoder_ButtonDoublePush;
  rotEncoder.onButtonLongPush = on_encoder_ButtonLongPush;

  /* Enable the I2C Encoder V2 interrupts according to the previus attached callback */
  rotEncoder.autoconfigInterrupt();

  rotEncoder2.reset();
  rotEncoder2.begin(i2cEncoderMiniLib::WRAP_ENABLE | i2cEncoderMiniLib::DIRE_LEFT | i2cEncoderMiniLib::IPUP_ENABLE | i2cEncoderMiniLib::RMOD_X1);
  rotEncoder2.writeCounter((int32_t)0);  /* Set the step to 1*/
  rotEncoder2.writeDoublePushPeriod(50); /*Set a period for the double push of 500ms */
  rotEncoder2.writeMax((int32_t)64);     /* Set the maximum threshold*/
  rotEncoder2.writeMin((int32_t)-64);    /* Set the minimum threshold */
  rotEncoder2.writeStep((int32_t)1);     /* Set the step to 1*/

  rotEncoder2.onChange = on_encoder_Change;
  rotEncoder2.onButtonPush = on_encoder_ButtonPush;
  rotEncoder2.onButtonRelease = on_encoder_ButtonRelease;
  rotEncoder2.onButtonDoublePush = on_encoder_ButtonDoublePush;
  rotEncoder2.onButtonLongPush = on_encoder_ButtonLongPush;

  /* Enable the I2C Encoder V2 interrupts according to the previus attached callback */
  rotEncoder2.autoconfigInterrupt();
}
void on_encoder_Change(i2cEncoderMiniLib *obj)
{
  encoderPosition = obj->readCounterLong();
  if (GuiStuff::activeGui != NULL)
  {
    GuiStuff::activeGui->handleEncoderChange(obj, encoderPosition);
  }
  Serial.println("Encoder is moved!");
  Serial.println(encoderPosition);
}

void on_encoder_ButtonPush(i2cEncoderMiniLib *obj)
{
  if (GuiStuff::activeGui != NULL)
  {
    GuiStuff::activeGui->handleButtonPush(obj);
  }
}
void on_encoder_ButtonRelease(i2cEncoderMiniLib *obj)
{
  if (GuiStuff::activeGui != NULL)
  {
    GuiStuff::activeGui->handleButtonRelease(obj);
  }
}
void on_encoder_ButtonDoublePush(i2cEncoderMiniLib *obj)
{
  if (GuiStuff::activeGui != NULL)
  {
    GuiStuff::activeGui->handleButtonDoublePush(obj);
  }
}
void on_encoder_ButtonLongPush(i2cEncoderMiniLib *obj)
{
  if (GuiStuff::activeGui != NULL)
  {
    GuiStuff::activeGui->handleButtonLongPush(obj);
  }
}

void PrintHEX(uint8_t i)
{
  Serial.print("0x");
  if (i < 16)
    Serial.print("0");
  Serial.print(i, HEX);
}
