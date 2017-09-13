#ifndef Joystick_h
#define Joystick_h

#include "Arduino.h"
#include "Sensor.h"
#include <LinkedList.h>
#include <Bounce2.h>

class Joystick
{
  public:
    Joystick(int _joystickNum);
    void update();
    void potInput();
    void buttonInput();
  private:
    int joystickNum;
    int sample[4];
    int center[2];

    Sensor * pots[2];
    LinkedList<Sensor*> sensors;

    Bounce * button = new Bounce;
};

#endif
