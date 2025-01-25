#ifndef GUIINTERFACE_H
#define GUIINTERFACE_H
#include <Adafruit_SSD1306.h>
#include <i2cEncoderMiniLib.h>

class GuiInterface
{
public:
  virtual void init(Adafruit_SSD1306 *dp, i2cEncoderMiniLib *encLeft, i2cEncoderMiniLib *encRight); // called once after creation
  virtual void setup();                                            // called when this GuiInterface replaces another

  virtual void handleButtonPush(i2cEncoderMiniLib *obj) {};
  virtual void handleButtonLongPush(i2cEncoderMiniLib *obj) {};
  virtual void handleButtonRelease(i2cEncoderMiniLib *obj) {};
  virtual void handleButtonDoublePush(i2cEncoderMiniLib *obj) {};
  virtual void handleEncoderChange(i2cEncoderMiniLib *obj, int32_t position) {};
  virtual void draw();

protected:
  Adafruit_SSD1306 *display;  // required for draw
  i2cEncoderMiniLib *encoderLeft; // required for userinput
  i2cEncoderMiniLib *encoderRight; // required for userinput
};




#endif
