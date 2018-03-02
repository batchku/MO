#ifndef MOBasic_h
#define MOBasic_h

#include "Arduino.h"
#include <Bounce2.h>

class MOBasic
{
  public:
    MOBasic();
    void setup();
    void update();
    void buttonInput();
    void potInput();
    void setLED();

  private:
  
   Bounce * buttonDebounce;
   int prevValue[3];
   int value;
};

#endif
