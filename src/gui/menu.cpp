#include "menu.h"
#include "guiStuff.h"

void Menu::draw()
{
  //Serial.println("draw menu");
  display->clearDisplay();
  display->setCursor(0, 0);
  display->setTextColor(WHITE, BLACK);

  // there is space for 4 items on the screen. The second one is the selected one
  for (int8_t i = 0; i < 4; i++)
  {
    int8_t tempIndex = selectedMenuEntryIndex + i - 1;
    if (tempIndex < 0 || menuEntries.size() <= tempIndex)
    {
      // print empty line
      display->println();
    }
    else
    {
      if (tempIndex == selectedMenuEntryIndex)
      {
        display->setTextColor(BLACK, WHITE); // invert color
        display->println(menuEntries[tempIndex]);
        display->setTextColor(WHITE, BLACK); // revert color
      }
      else
      {
        display->println(menuEntries[tempIndex]);
      }
    }
  }

  display->display();
}

void Menu::init(Adafruit_SSD1306 *dp, i2cEncoderMiniLib *encLeft, i2cEncoderMiniLib *encRight)
{
  display = dp;
  encoderLeft = encLeft;
  encoderRight = encRight;
  setup();
}

void Menu::setup()
{
  encoderLeft->begin(i2cEncoderMiniLib::WRAP_ENABLE | i2cEncoderMiniLib::DIRE_LEFT | i2cEncoderMiniLib::IPUP_ENABLE | i2cEncoderMiniLib::RMOD_X1);
  encoderLeft->writeMin((int32_t)0);                      /* Set the minimum threshold */
  encoderLeft->writeMax((int32_t)menuEntries.size() - 1); /* Set the maximum threshold*/
  encoderLeft->writeStep((int32_t)1);                     /* Set the step to 1*/
  encoderLeft->writeCounter(selectedMenuEntryIndex);

  display->setTextSize(2);
  draw();
}

void Menu::handleButtonPush(i2cEncoderMiniLib *obj)
{
  if (obj == encoderLeft){
    //Serial.println("Menu item selected");
    GuiStuff::setActiveGui(guiEntries.at(selectedMenuEntryIndex));
  }
}

void Menu::handleEncoderChange(i2cEncoderMiniLib *obj, int32_t position)
{
  if (obj == encoderLeft){
    selectedMenuEntryIndex = position;
    draw();
  }
}

void Menu::addMenuEntry(String name, GuiInterface *gui)
{
  menuEntries.push_back(name);
  guiEntries.push_back(gui);
}
