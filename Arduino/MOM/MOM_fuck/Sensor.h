#ifndef Sensor_h
#define Sensor_h


#include "Arduino.h"

class Sensor
{
  public:
    Sensor();
    Sensor(int _input, int _midiAddress);
    void set(int _input, int _midiAddress);
    void setInput(int _input);
    void setMidi(int _midiAddress);
    void update();

  private:
    int input;
    int midiAddress;
    int potPrevs[3];
    int potSend;
};

#endif
