#ifndef TOYINTERFACE_H
#define TOYINTERFACE_H

enum Intensity {
  Off,
  Low,
  High
};
class ToyInterface {

public:
  virtual void setIntensity(Intensity intensity);
};

#endif
