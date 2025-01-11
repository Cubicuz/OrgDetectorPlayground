#include "settings.h"
#include "../preferencesManager.h"
#include "guiStuff.h"
#include "toy/toyBle.h"

enum SETTINGS_INDEX : uint8_t {
  IndexReturn,
  IndexBtEnable,
  IndexBtDeviceSelect,
  IndexBtMaxIntensity,
  IndexPwmEnable,
  INDEXSIZE // this is always the last index
};

void Settings::draw()
{
  Serial.print("draw Settings\n");
  display->clearDisplay();
  display->setCursor(0, 0);
  display->setTextSize(1);
  display->setTextColor(WHITE, BLACK);
  // there is space for 4 items on the screen. The second one is the selected one
  for (int8_t i = 0; i < 7; i++)
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

void Settings::init(Adafruit_SSD1306 *dp, i2cEncoderMiniLib *enc)
{
  display = dp;
  encoder = enc;
}

void Settings::setup()
{
  setEncoderPurpose(menu);
  draw();
}


void Settings::handleButtonLongPush()
{
}

void Settings::handleEncoderChange(int32_t position)
{
  switch (encoderMode){
    case menu:
      selectedIndex = position;
      draw();
      break;
    case BtVibIntensity:
    case BtDeviceSelect:
    case PwmLowIntensity:
    case PwmHighIntensity:
      tempEncoderVal = position;
      draw();
      break;
    default:
      break;
  }
}

void Settings::handleButtonPush()
{
  switch(selectedIndex){
    case IndexReturn: 
      GuiStuff::setActiveGui(&GuiStuff::guiMenu);
      break;
    case IndexBtEnable: // toggle bluetooth enabled
      PreferencesManager::instance.setBluetoothEnabled(!PreferencesManager::instance.bluetoothEnabled());
      draw();
      break;
    case IndexBtDeviceSelect:
      if (encoderMode == menu) {
        setEncoderPurpose(BtDeviceSelect);
      } else {
        PreferencesManager::instance.setSelectedBluetoothDeviceIndex(tempEncoderVal);
        setEncoderPurpose(menu);
      }
      draw();
      break;
    case IndexPwmEnable: // toggle bluetooth enabled
      PreferencesManager::instance.setPwmToyEnabled(!PreferencesManager::instance.pwmToyEnabled());
      draw();
      break;
    case IndexBtMaxIntensity:
      if (encoderMode == menu) {
        setEncoderPurpose(BtVibIntensity);
      } else {
        PreferencesManager::instance.setBloetoothIntensity(tempEncoderVal);
        setEncoderPurpose(menu);
      }
      draw();
    default:
      break;
  }
}
void Settings::setEncoderPurpose(EncoderPurpose e)
{
  encoderMode = e;
  switch (e)
  {
  case menu:
    encoder->begin(i2cEncoderMiniLib::WRAP_ENABLE | i2cEncoderMiniLib::DIRE_LEFT | i2cEncoderMiniLib::IPUP_ENABLE | i2cEncoderMiniLib::RMOD_X1);
    encoder->writeMin((int32_t)0);                      /* Set the minimum threshold */
    encoder->writeMax((int32_t)INDEXSIZE - 1); /* Set the maximum threshold*/
    encoder->writeStep((int32_t)1);                     /* Set the step to 1*/
    encoder->writeCounter(selectedIndex);
    tempEncoderVal = 0;
    break;
  case BtVibIntensity:
    encoder->begin(i2cEncoderMiniLib::WRAP_ENABLE | i2cEncoderMiniLib::DIRE_LEFT | i2cEncoderMiniLib::IPUP_ENABLE | i2cEncoderMiniLib::RMOD_X1);
    encoder->writeMin((int32_t)0);                      /* Set the minimum threshold */
    encoder->writeMax((int32_t)15); /* Set the maximum threshold*/
    encoder->writeStep((int32_t)1);                     /* Set the step to 1*/
    encoder->writeCounter(PreferencesManager::instance.bluetoothIntensity());
    tempEncoderVal = PreferencesManager::instance.bluetoothIntensity();
    break;
  case BtDeviceSelect:
    encoder->begin(i2cEncoderMiniLib::WRAP_ENABLE | i2cEncoderMiniLib::DIRE_LEFT | i2cEncoderMiniLib::IPUP_ENABLE | i2cEncoderMiniLib::RMOD_X1);
    encoder->writeMin((int32_t)0);                      /* Set the minimum threshold */
    encoder->writeMax((int32_t)ToyBLE::selectableDevices.size()-1); /* Set the maximum threshold*/
    encoder->writeStep((int32_t)1);                     /* Set the step to 1*/
    tempEncoderVal = PreferencesManager::instance.selectedBluetoothDeviceIndex();
    encoder->writeCounter(tempEncoderVal);
    break;  
  case PwmLowIntensity:
    encoder->begin(i2cEncoderMiniLib::WRAP_ENABLE | i2cEncoderMiniLib::DIRE_LEFT | i2cEncoderMiniLib::IPUP_ENABLE | i2cEncoderMiniLib::RMOD_X1);
    encoder->writeMin((int32_t)0);                      /* Set the minimum threshold */
    encoder->writeMax((int32_t)PreferencesManager::instance.pwmToyHighIntensity()); /* Set the maximum threshold*/
    encoder->writeStep((int32_t)1);                     /* Set the step to 1*/
    encoder->writeCounter(PreferencesManager::instance.pwmToyLowIntensity());
    tempEncoderVal = PreferencesManager::instance.pwmToyLowIntensity();
    break;
  case PwmHighIntensity:
    encoder->begin(i2cEncoderMiniLib::WRAP_ENABLE | i2cEncoderMiniLib::DIRE_LEFT | i2cEncoderMiniLib::IPUP_ENABLE | i2cEncoderMiniLib::RMOD_X1);
    encoder->writeMin((int32_t)PreferencesManager::instance.pwmToyLowIntensity());                      /* Set the minimum threshold */
    encoder->writeMax((int32_t)255); /* Set the maximum threshold*/
    encoder->writeStep((int32_t)1);                     /* Set the step to 1*/
    encoder->writeCounter(PreferencesManager::instance.pwmToyHighIntensity());
    tempEncoderVal = PreferencesManager::instance.pwmToyHighIntensity();
    break;
  default:
    break;
  }
}
void Settings::drawIndex(uint8_t index)
{
  switch (index){
    case IndexReturn: 
      display->println("return");
      break;
    case IndexBtEnable: // enable/disable bluetooth
      if (PreferencesManager::instance.bluetoothEnabled()){
        display->println("BT on");
      } else {
        display->println("BT off");
      }
      break;
    case IndexBtDeviceSelect:
      if (encoderMode == BtDeviceSelect){
        display->printf("* %.10s", ToyBLE::selectableDevices[tempEncoderVal].displayName);
        display->println();
      } else {
        display->printf("BtDev: %.10s", ToyBLE::selectableDevices[PreferencesManager::instance.selectedBluetoothDeviceIndex()].displayName);
        display->println();
      }
    case IndexPwmEnable: // enable/disable pwm
      if (PreferencesManager::instance.pwmToyEnabled()){
        display->println("PWM on");
      } else {
        display->println("PWM off");
      }
      break;
    case IndexBtMaxIntensity: // set the intensity of the vibration
      if (encoderMode != BtVibIntensity){
        display->print("vib ");
        display->println(PreferencesManager::instance.bluetoothIntensity());
      } else {
        display->print("*vib ");
        display->println(tempEncoderVal);
      }
    default:
      break;
  }
}
