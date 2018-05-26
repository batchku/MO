#ifndef Strip_h
#define Strip_h
#define NUM_CONTROLS  2

#include "Arduino.h"
#include <Bounce2.h>
#include <LinkedList.h>
#include "Sensor.h"
#include <SoftPWM.h>

class Strip
{
  public:
    Strip(int _stripNum);
    void update();
    void add(Sensor *sensor);
    void potInput();
    void buttonInput();
    void light(int note, int velocity);
  private:
    int potPrevs[3][NUM_CONTROLS]; //used to store previous values for pots
    int potSends[NUM_CONTROLS];
    
    Sensor pots[NUM_CONTROLS];
    LinkedList<Sensor*> sensors;
    
    int stripNum;
    Bounce * button = new Bounce[NUM_CONTROLS]; // Create Bounce objects for each button.
    elapsedMillis msec;    
};

#endif
