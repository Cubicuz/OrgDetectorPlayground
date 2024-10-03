#include <Arduino.h>
#include <Wire.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <i2cEncoderMiniLib.h>

#include "gui/guiStuff.h"
#include "toy/toyManager.h"
#include "sensor/adcManager.h"
#include "preferencesManager.h"

#define I2C_0_SDA 16
#define I2C_0_SCL 17
#define I2C_0_CLOCK 100000
#define IntPin 19

Adafruit_SSD1306 display(128, 64, &Wire); // width, height
i2cEncoderMiniLib rotEncoder(0x20);

ToyManager toyManager;
ADCManager adcManager;

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
  ToyManager::Instance = &toyManager;


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


      adcManager.init();
      toyManager.init();
      PreferencesManager::instance.begin();
    }
  }
}
uint16_t delayCtr = 0;
void loop()
{
  delayCtr++;
  // put your main code here, to run repeatedly:
  
  rotEncoder.updateStatus();
  
  if ((GuiStuff::activeGui == &GuiStuff::guiPlay) && adcManager.updateValues()){
    GuiStuff::guiPlay.setAdcValue(adcManager.getAdcValue());
  }
  if (delayCtr == 1000) { // check bluetooth once per 5 seconds
    toyManager.checkConnections();
    delayCtr = 0;
  }
  
  delay(5);
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
