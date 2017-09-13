
#ifndef Shield_h
#define Shield_h
#define NUM_USERCONTROLS  4

#include "Arduino.h"
#include <Bounce2.h>
#include <LinkedList.h>
#include "Sensor.h"

class Shield
{
  public:
    Shield();
    void update();
    void add(Sensor *sensor);
    void potInput();
    void buttonInput();
    void light(int note, int velocity);
  private:
    Sensor * pots[NUM_USERCONTROLS];

    LinkedList<Sensor*> sensors;
    
    Bounce * button = new Bounce[NUM_USERCONTROLS]; // Create Bounce objects for each button.
    elapsedMillis msec;    
};

#endif
