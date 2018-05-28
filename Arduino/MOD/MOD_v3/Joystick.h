#ifndef Joystick_h
#define Joystick_h

#include "Arduino.h"
#include <LinkedList.h>
#include <Bounce2.h>
#include "Sensor.h"

class Joystick
{
  public:
    Joystick(int _joystickNum);
    void update();
    void calibrate();
    void potInput();
    void buttonInput();
    void sendMIDI(int which, int val);
  private:
    int joystickNum;
    //int sample[4];
    int center[2];
    int current[2];
    int lastSentVal[2];

    Sensor pots[2];
   

    Bounce * button = new Bounce;
    elapsedMillis timer;  
};

#endif
