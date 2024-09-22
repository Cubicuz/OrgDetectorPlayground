#ifndef GUIINTERFACE_H
#define GUIINTERFACE_H
#include <Adafruit_SSD1306.h>
#include <i2cEncoderMiniLib.h>

class GuiInterface
{
public:
  virtual void init(Adafruit_SSD1306 *dp, i2cEncoderMiniLib *enc); // called once after creation
  virtual void setup();                                            // called when this GuiInterface replaces another

  virtual void handleUserInput() {};
  virtual void handleButtonPush() {};
  virtual void handleButtonLongPush() {};
  virtual void handleEncoderChange(int32_t position) {};
  virtual void draw();

protected:
  Adafruit_SSD1306 *display;  // required for draw
  i2cEncoderMiniLib *encoder; // required for userinput
};




#endif
