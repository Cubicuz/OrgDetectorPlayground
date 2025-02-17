#include "settings.h"
#include "../preferencesManager.h"
#include "guiStuff.h"
#include "toy/toyBle.h"
#include "../sensor/adcManager.h"

enum SettingsIndex : uint8_t {
  IndexReturn,
  IndexBtEnable,
  IndexBtDeviceSelect,
  IndexBtMaxIntensity,
  IndexPwmEnable,
  IndexAdcRef,
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

void Settings::init(Adafruit_SSD1306 *dp, i2cEncoderMiniLib *encLeft, i2cEncoderMiniLib *encRight)
{
  display = dp;
  encoderLeft = encLeft;
  encoderRight = encRight;
}

void Settings::setup()
{
  setEncoderPurpose(menu);
  draw();
}


void Settings::handleButtonLongPush(i2cEncoderMiniLib *obj)
{
}

void Settings::handleEncoderChange(i2cEncoderMiniLib *obj, int32_t position)
{
  if (obj == encoderLeft){
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
}

void Settings::handleButtonPush(i2cEncoderMiniLib *obj)
{
  if (obj == encoderLeft){
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
        break;
      case IndexAdcRef:
        ADCManager::instance.updateValues();
        PreferencesManager::instance.setAdcReference1Bar(ADCManager::instance.getAdcValue());
        draw();
        break;
      default:
        break;
    }
  }
}
void Settings::setEncoderPurpose(EncoderPurpose e)
{
  encoderMode = e;
  switch (e)
  {
  case menu:
    encoderLeft->begin(i2cEncoderMiniLib::WRAP_ENABLE | i2cEncoderMiniLib::DIRE_LEFT | i2cEncoderMiniLib::IPUP_ENABLE | i2cEncoderMiniLib::RMOD_X1);
    encoderLeft->writeMin((int32_t)0);                      /* Set the minimum threshold */
    encoderLeft->writeMax((int32_t)INDEXSIZE - 1); /* Set the maximum threshold*/
    encoderLeft->writeStep((int32_t)1);                     /* Set the step to 1*/
    encoderLeft->writeCounter(selectedIndex);
    tempEncoderVal = 0;
    break;
  case BtVibIntensity:
    encoderLeft->begin(i2cEncoderMiniLib::WRAP_ENABLE | i2cEncoderMiniLib::DIRE_LEFT | i2cEncoderMiniLib::IPUP_ENABLE | i2cEncoderMiniLib::RMOD_X1);
    encoderLeft->writeMin((int32_t)0);                      /* Set the minimum threshold */
    encoderLeft->writeMax((int32_t)15); /* Set the maximum threshold*/
    encoderLeft->writeStep((int32_t)1);                     /* Set the step to 1*/
    encoderLeft->writeCounter(PreferencesManager::instance.bluetoothIntensity());
    tempEncoderVal = PreferencesManager::instance.bluetoothIntensity();
    break;
  case BtDeviceSelect:
    encoderLeft->begin(i2cEncoderMiniLib::WRAP_ENABLE | i2cEncoderMiniLib::DIRE_LEFT | i2cEncoderMiniLib::IPUP_ENABLE | i2cEncoderMiniLib::RMOD_X1);
    encoderLeft->writeMin((int32_t)0);                      /* Set the minimum threshold */
    encoderLeft->writeMax((int32_t)ToyBLE::selectableDevices.size()-1); /* Set the maximum threshold*/
    encoderLeft->writeStep((int32_t)1);                     /* Set the step to 1*/
    tempEncoderVal = PreferencesManager::instance.selectedBluetoothDeviceIndex();
    encoderLeft->writeCounter(tempEncoderVal);
    break;  
  case PwmLowIntensity:
    encoderLeft->begin(i2cEncoderMiniLib::WRAP_ENABLE | i2cEncoderMiniLib::DIRE_LEFT | i2cEncoderMiniLib::IPUP_ENABLE | i2cEncoderMiniLib::RMOD_X1);
    encoderLeft->writeMin((int32_t)0);                      /* Set the minimum threshold */
    encoderLeft->writeMax((int32_t)PreferencesManager::instance.pwmToyHighIntensity()); /* Set the maximum threshold*/
    encoderLeft->writeStep((int32_t)1);                     /* Set the step to 1*/
    encoderLeft->writeCounter(PreferencesManager::instance.pwmToyLowIntensity());
    tempEncoderVal = PreferencesManager::instance.pwmToyLowIntensity();
    break;
  case PwmHighIntensity:
    encoderLeft->begin(i2cEncoderMiniLib::WRAP_ENABLE | i2cEncoderMiniLib::DIRE_LEFT | i2cEncoderMiniLib::IPUP_ENABLE | i2cEncoderMiniLib::RMOD_X1);
    encoderLeft->writeMin((int32_t)PreferencesManager::instance.pwmToyLowIntensity());                      /* Set the minimum threshold */
    encoderLeft->writeMax((int32_t)255); /* Set the maximum threshold*/
    encoderLeft->writeStep((int32_t)1);                     /* Set the step to 1*/
    encoderLeft->writeCounter(PreferencesManager::instance.pwmToyHighIntensity());
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
        display->printf("* %.10s", ToyBLE::selectableDevices[tempEncoderVal].displayName.c_str());
        display->println();
      } else {
        display->printf("BtDev: %.10s", ToyBLE::selectableDevices[PreferencesManager::instance.selectedBluetoothDeviceIndex()].displayName.c_str());
        display->println();
      }
      break;
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
      break;
    case IndexAdcRef: // set the adc reference to detect negative pressures
      display->printf("AdcRef: %" PRIu16 "\r\n", PreferencesManager::instance.adcReference1Bar());
      break;
    default:
      break;
  }
}
