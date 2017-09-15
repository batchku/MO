#ifndef Sensor_h
#define Sensor_h


#include "Arduino.h"

class Sensor
{
  public:
    Sensor();
    Sensor(const int  _input, const int _midiAddress);
    void set(int _input, int _midiAddress);
    void setInput(int _input);
    void setMidi(int _midiAddress);
    void update();
    void read();
    void send();
    int getCurrent();
    void setName(String _name);
    void debug();

  private:
    int input;
    int midiAddress;
    int current;
    int thresh;
    int potPrev[3];
    int potAvg;
    bool dataChange;
    String name;
    elapsedMillis timer;
};

#endif




