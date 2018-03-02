#ifndef Strip_h
#define Strip_h

#include "Arduino.h"
#include <Bounce2.h>

class Strip
{
  public:
    Strip(int stripNum);
    void setup();
    void update();
    void buttonInput();
    void potInput();
    void setLED();

    int pot[2];
    int button[2];
    int led[3];
  private:
    int _stripNum;
};

#endif
