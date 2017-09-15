#ifndef JoystickCollection_h
#define JoystickCollection_h

#include "Arduino.h"
#include "Joystick.h"
#include <LinkedList.h>

class JoystickCollection
{
  public:
    JoystickCollection();
    void update();
    void add(Joystick * joystick);
    void calibrate();
  private:
    LinkedList<Joystick*> joystickList;
};

#endif
