#include "bluetoothSettings.h"
#include "../preferencesManager.h"
#include "guiStuff.h"

static const uint8_t INDEXRETURN = 0;
static const uint8_t INDEXENABLED = 1;
static const int8_t INDEXSIZE = 2;
void BluetoothSettings::draw()
{
  Serial.print("draw bluetoothsettings\n");
  display->clearDisplay();
  display->setCursor(0, 0);
  display->setTextSize(2);
  display->setTextColor(WHITE, BLACK);
  // there is space for 4 items on the screen. The second one is the selected one
  for (int8_t i = 0; i < 4; i++)
  {
    int8_t tempIndex = selectedIndex + i - 1;
    if (tempIndex < 0 || INDEXSIZE <= tempIndex)
    {
      // print empty line
      display->println("");
    }
    else
    {
      if (tempIndex == selectedIndex)
      {
        display->setTextColor(BLACK, WHITE); // invert color
        drawIndex(tempIndex);
        display->setTextColor(WHITE, BLACK); // revert color
      }
      else
      {
        drawIndex(tempIndex);
      }
    }
  }
  display->display();
}

void BluetoothSettings::init(Adafruit_SSD1306 *dp, i2cEncoderMiniLib *enc)
{
  display = dp;
  encoder = enc;
}

void BluetoothSettings::setup()
{
  encoder->begin(i2cEncoderMiniLib::WRAP_ENABLE | i2cEncoderMiniLib::DIRE_LEFT | i2cEncoderMiniLib::IPUP_ENABLE | i2cEncoderMiniLib::RMOD_X1);
  encoder->writeMin((int32_t)0);                      /* Set the minimum threshold */
  encoder->writeMax((int32_t)INDEXSIZE - 1); /* Set the maximum threshold*/
  encoder->writeStep((int32_t)1);                     /* Set the step to 1*/
  encoder->writeCounter(selectedIndex);

  draw();
}


void BluetoothSettings::handleButtonLongPush()
{
}

void BluetoothSettings::handleEncoderChange(int32_t position)
{
  selectedIndex = position;
  draw();
}

void BluetoothSettings::handleButtonPush()
{
  switch(selectedIndex){
    case INDEXRETURN: 
      GuiStuff::setActiveGui(&GuiStuff::guiMenu);
      break;
    case INDEXENABLED: // toggle bluetooth enabled
      PreferencesManager::instance.setBluetoothEnabled(!PreferencesManager::instance.bluetoothEnabled());
      draw();
      break;
    default:
      break;
  }
}
void BluetoothSettings::drawIndex(uint8_t index)
{
  switch (index){
    case INDEXRETURN: 
      display->println("return");
      break;
    case INDEXENABLED: // enable/disable bluetooth
      if (PreferencesManager::instance.bluetoothEnabled()){
        display->println("enabled");
      } else {
        display->println("disabled");
      }
      break;
    default:
      break;
  }
}
